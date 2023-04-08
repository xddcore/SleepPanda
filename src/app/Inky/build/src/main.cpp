#include "Inky.hpp"
#include "HttpService.hpp"
#include "Image.hpp"
#include "ImageIO.hpp"
#include "Draw.hpp"
#include "QRCode.hpp"

#include <gpio-cpp/gpio.hpp>
#include <magic_enum.hpp>
#include <fmt/format.h>
#include <signal.h>

#include <iostream>
#include <random>

using namespace magic_enum::ostream_operators;
using namespace httplib;

volatile bool interrupt_received = false;
volatile bool internal_exit = false;

static void InterruptHandler(int signo)
{
    interrupt_received = true;
}

int main(int argc, char *argv[])
{
  // Subscribe to signal interrupts
  signal(SIGTERM, InterruptHandler);
  signal(SIGINT, InterruptHandler);

  auto display = Inky::Create();
  std::cout << "Connected to Inky Display!" << std::endl;
  std::cout << "\tWidth: " << display->info().width << std::endl;
  std::cout << "\tHeight: " << display->info().height << std::endl;
  std::cout << "\tColor Capability: " << display->info().colorCapability << std::endl;
  std::cout << "\tPCB Variant: " << (int)display->info().pcbVariant << std::endl;
  std::cout << "\tDisplay Variant: " << display->info().displayVariant << std::endl;
  std::cout << "\tWrite Time: " << display->info().writeTime << std::endl;

  // Http Service Setup
  HttpService http;
  http.Server().Get("/current_photo.png",
  [&](const Request &req, Response &res) 
  {
    std::string imgBuf;
    ImageIO::SaveToBuffer(display->getImage(), imgBuf, {.saveFormat = ImageFormat::PNG});
    res.set_content(imgBuf, "image/png");
  });

  http.Server().Post("/post_photo",
  [&](const Request &req, Response &res) 
  {
    if (req.is_multipart_form_data()) 
    {
        // Process images here...
        std::cout << "Got " << req.files.size() << " files in multipart request!" << std::endl; 
        for (const auto& [name, data] : req.files)
        {
            std::cout << "Name: " << data.name 
                      << " | Filename: " << data.filename 
                      << " | Content Type: " << data.content_type
                      << " | Content Size: " << data.content.size()
                      << std::endl;
            if (data.content_type.find("image/") == 0)
            {
                std::cout << "Image appears to be an image, sending to display..." << std::endl;
                Image newImage = ImageIO::LoadFromBuffer(data.content);
                newImage.scale(display->info().width, display->info().height, {.scaleMode = ScaleMode::Fill});
                display->setImage(newImage);
                display->show();
                break;
            }
        }
        res.set_redirect("/");
    }
    else 
    {
    //   std::string contentType = "UNDEFINED";
    //   if (req.has_header("Content-Type")) 
    //   {
    //     contentType = req.get_header_value("Content-Type");
    //   }

    //   // Process image here...
    //   std::cout << "Got simple post request!" << std::endl;
    //   std::cout << "Content Type: " << contentType
    //             << " | Body Size: " << req.body.size()
    //             << std::endl;
    //   std::cout << "Printing first 255 chars of body:" << std::endl; 
    //   std::cout << req.body.substr(0, 255) << std::endl; 
    }
  });

  // GPIO Setup
  Gpio gpio;
  gpio.setupLine(5, Gpio::LineMode::Input, Gpio::LineBias::PullUp);
  gpio.setupLine(6, Gpio::LineMode::Input, Gpio::LineBias::PullUp);
  gpio.setupLine(16, Gpio::LineMode::Input, Gpio::LineBias::PullUp);
  gpio.setupLine(24, Gpio::LineMode::Input, Gpio::LineBias::PullUp);

  gpio.subscribe(5, [&](int line, Gpio::LineTransition transition, std::chrono::steady_clock::time_point timestamp)
  {
    if (transition == Gpio::LineTransition::FallingEdge)
    {
      // Get the URL to show from the HttpService
      std::string configURL = fmt::format("http://{}", http.ListeningInterface());
      auto qrCode = QRCode::GenerateImage(configURL);
      qrCode.scale(display->info().width, display->info().height-50, {.scaleMode = ScaleMode::Fit, .interpolationMode = InterpolationMode::Nearest});
      qrCode.crop(0, 0, display->info().width, display->info().height);
      Draw::Text(qrCode, display->info().width / 2, display->info().height-50, configURL, {.hAlign = Draw::HAlign::Center});
      Draw::Text(qrCode, display->info().width / 2, display->info().height-30, "Scan the QR code to upload a new photo.", {.hAlign = Draw::HAlign::Center});
      display->setImage(qrCode, {.scaleMode = ScaleMode::Fill}, {.ditherAccuracy = 0});
      display->show();
    }
  });

  gpio.subscribe(6, [&](int line, Gpio::LineTransition transition, std::chrono::steady_clock::time_point timestamp)
  {
    if (transition == Gpio::LineTransition::FallingEdge)
    {
      display->show(Inky::ShowOperation::ColorTest);
    }
  });

  gpio.subscribe(16, [&](int line, Gpio::LineTransition transition, std::chrono::steady_clock::time_point timestamp)
  {
    if (transition == Gpio::LineTransition::FallingEdge)
    {
      Image img(display->info().width, display->info().height);

      std::random_device dev;
      std::mt19937 rng(dev());
      std::uniform_int_distribution<std::mt19937::result_type> randomHue(0,360);
      std::uniform_int_distribution<std::mt19937::result_type> randomX(0,img.width());
      std::uniform_int_distribution<std::mt19937::result_type> randomY(0,img.height());
      std::uniform_int_distribution<std::mt19937::result_type> randomW(100,200);
      std::uniform_int_distribution<std::mt19937::result_type> randomH(100,200);

      HSVColor bgColor = { (float)randomHue(rng), 0.8f, 0.75f };
      Draw::Box(img, 0, 0, img.width(), img.height(), {.color = bgColor.toRGB()});

      for (int i=0; i < 16; ++i)
      {
        HSVColor color { (float)randomHue(rng), 0.8f, 0.75f };
        Draw::Box(img, randomX(rng), randomY(rng), randomW(rng), randomH(rng), 
                  {.hAlign = Draw::HAlign::Center, .vAlign = Draw::VAlign::Center, .color = color.toRGB()});
      }

      Draw::Box(img, img.width()/2, img.height()/2, 340, 88, {.hAlign = Draw::HAlign::Center, .vAlign = Draw::VAlign::Center, .color = {128,255,128}});

      // Convert to indexed here so we can dither the boxes and not the text
      img.toIndexed(display->getColorMap(), {.ditherAccuracy = 0.8f});

      Draw::Text(img, img.width()/2, img.height()/2 - 41, "Inky Frame", {.hAlign = Draw::HAlign::Center, .font = Draw::Font::Mono_32x48});
      Draw::Text(img, img.width()/2, img.height()/2 + 12, "Powered by inky-cpp", {.hAlign = Draw::HAlign::Center, .font = Draw::Font::Mono_8x12});
      Draw::Text(img, img.width()/2, img.height()/2 + 28, "https://github.com/DonkeyKong/inky-cpp", {.hAlign = Draw::HAlign::Center, .font = Draw::Font::Mono_8x12});
      
      // Make sure the dither accuracy is set to 0 to ensure sharp pixel exact text
      display->setImage(img, {.scaleMode = ScaleMode::Fill}, {.ditherAccuracy = 0}); 
      
      display->show();
    }
  });

  gpio.subscribe(24, [&](int line, Gpio::LineTransition transition, std::chrono::steady_clock::time_point timestamp)
  {
    if (transition == Gpio::LineTransition::FallingEdge)
    {
      display->show(Inky::ShowOperation::CleanDisplay);
    }
  });

  // Start the main waiting loop
  while (!interrupt_received && !internal_exit)
  {
      // Regulate update rate
      std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  if (interrupt_received)
  {
      fprintf(stderr, "Main thread caught exit signal.\n");
  }

  if (internal_exit)
  {
      fprintf(stderr, "Main thread got internal exit request.\n");
  }
}
