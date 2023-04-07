#include "HttpService.hpp"

#include <sys/types.h>
#include <ifaddrs.h>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <fmt/format.h>

using json = nlohmann::json;

static bool endsWith(const std::string &fullString, const std::string &ending)
{
  if (fullString.length() >= ending.length())
  {
    return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
  }
  return false;
}

static std::string getMimeType(const std::string &filename)
{
  if (endsWith(filename, ".html") || endsWith(filename, ".htm"))
  {
    return "text/html";
  }
  else if (endsWith(filename, ".css"))
  {
    return "text/css";
  }
  else if (endsWith(filename, ".js"))
  {
    return "text/javascript";
  }

  return "text/plain";
}

static std::string getFirstExternalHostAddr()
{
  std::string hostAddr = "0.0.0.0";

  struct ifaddrs *ifaddr;
  char host[NI_MAXHOST];
  const char *loopbackPrefix = "lo";

  if (getifaddrs(&ifaddr) != -1)
  {
    /* Walk through linked list, maintaining head pointer so we
        can free list later. */

    for (struct ifaddrs *ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next)
    {
      // Skip any interfaces without addresses
      if (ifa->ifa_addr == nullptr)
        continue;

      // Skip interfaces that aren't IPv4
      if (ifa->ifa_addr->sa_family != AF_INET)
        continue;

      // Skip loopback interfaces (we need to connect form outside!)
      bool isLoopback = strncmp(loopbackPrefix, ifa->ifa_name, strlen(loopbackPrefix)) == 0;
      if (isLoopback)
        continue;

      int s = getnameinfo(ifa->ifa_addr,
                          sizeof(struct sockaddr_in),
                          host, NI_MAXHOST,
                          NULL, 0, NI_NUMERICHOST);
      if (s == 0)
      {
        // Return the first non-loopback, assigned interface
        hostAddr = std::string(host);
        break;
      }
    }
    freeifaddrs(ifaddr);
  }

  return hostAddr;
}

HttpService::HttpService()
{
  std::filesystem::path webDir = "resources";
  for (const auto &entry : std::filesystem::directory_iterator(webDir))
  {
    if (entry.is_regular_file() && (entry.path().extension() == ".html" ||
                                    entry.path().extension() == ".htm" ||
                                    entry.path().extension() == ".css" ||
                                    entry.path().extension() == ".js"))
    {
      std::ifstream t(entry.path());
      std::stringstream buffer;
      buffer << t.rdbuf();
      web[entry.path().filename()] = buffer.str();
    }
  }

  srv = std::make_unique<httplib::Server>();

  srv->set_payload_max_length(1024 * 1024 * 100); // 100MB

  // Setup the HTTP API
  setupCallbacks();

  // Figure out the port and bind address for the server
  // These cannot change so no need to subscribe
  int port = 80;
  std::string addr = "0.0.0.0";

  if (port == 0)
  {
    port = srv->bind_to_any_port(addr.c_str());

    serverThread = std::make_unique<std::thread>([=, this]()
                                                 { srv->listen_after_bind(); });
  }
  else
  {
    serverThread = std::make_unique<std::thread>([=, this]()
                                                 { srv->listen(addr.c_str(), port); });
  }

  // Wait for server to start
  int i = 0;
  while (srv->is_valid() && !srv->is_running() && i < 10)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  if (!srv->is_valid() || i == 10)
  {
    srv = nullptr;
    serverThread = nullptr;
    throw std::runtime_error("Mock server did not start!");
  }

  listeningInterface = getFirstExternalHostAddr();
}

HttpService::~HttpService()
{
  if (srv != nullptr)
  {
    srv->stop();
  }

  if (serverThread != nullptr)
  {
    serverThread->join();
  }
}

static json getRequestPayload(const httplib::Request &req)
{
  json payload;
  if (req.has_header("Content-Type") &&
      req.get_header_value("Content-Type") == "application/x-www-form-urlencoded")
  {
    for (const auto &[key, value] : req.params)
    {
      payload[key] = value;
    }
  }
  else // assume json
  {
    payload = json::parse(req.body);
  }
  return payload;
}

void HttpService::setupCallbacks()
{
  // Serve everything in the web server cache
  for (const auto &[filename, text] : web)
  {
    std::string name = filename;
    std::string mimetype = getMimeType(name);
    srv->Get(fmt::format("/{}", filename), [this, name, mimetype](const httplib::Request &req, httplib::Response &res)
             { res.set_content(web[name], mimetype); });
  }

  // Add the default index handler
  if (web.count("index.html"))
  {
    srv->Get("/", [=, this](const httplib::Request &req, httplib::Response &res)
             { res.set_content(web["index.html"], getMimeType("index.html")); });
  }
  else if (web.count("index.htm"))
  {
    srv->Get("/", [=, this](const httplib::Request &req, httplib::Response &res)
             { res.set_content(web["index.htm"], getMimeType("index.htm")); });
  }
}

std::string HttpService::ListeningInterface()
{
  listeningInterface = getFirstExternalHostAddr();
  return listeningInterface;
}

bool HttpService::Running()
{
  return srv->is_running();
}

httplib::Server &HttpService::Server()
{
  return *srv;
}
