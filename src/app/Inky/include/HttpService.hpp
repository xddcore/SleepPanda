#pragma once

#include <httplib.h>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <condition_variable>

class HttpService
{
public:
    HttpService();
    ~HttpService();
    bool Running();
    std::string ListeningInterface();
    httplib::Server& Server();
private:
    std::string listeningInterface;
    void setupCallbacks();
    std::unique_ptr<httplib::Server> srv;
    std::unique_ptr<std::thread> serverThread;
    std::unordered_map<std::string, std::string> web;
};