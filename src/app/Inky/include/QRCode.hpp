#pragma once

#include "Image.hpp"
#include <string>

namespace QRCode
{
  Image GenerateImage(const std::string& qrPayload);
}