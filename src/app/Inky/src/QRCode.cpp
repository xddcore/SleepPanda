#include "QRCode.hpp"

#include <qrcodegen.hpp>

namespace QRCode
{
  Image GenerateImage(const std::string& qrPayload)
  {
    auto qr = qrcodegen::QrCode::encodeText(qrPayload.c_str(), qrcodegen::QrCode::Ecc::MEDIUM);
    const int quietZoneSize = 2;
    Image image (qr.getSize() + quietZoneSize * 2, qr.getSize() + quietZoneSize * 2);
    auto dataPtr = image.data();
    for (int y = -quietZoneSize; y < qr.getSize() + quietZoneSize; y++)
    {
        for (int x = -quietZoneSize; x < qr.getSize() + quietZoneSize; x++)
        {
            bool pxIsDark = qr.getModule(x,y);
            dataPtr[0] = pxIsDark ? 0 : 255;
            dataPtr[1] = pxIsDark ? 0 : 255;
            dataPtr[2] = pxIsDark ? 0 : 255;
            dataPtr[3] = 255;
            dataPtr += 4;
        }
    }
    
    return image;
  }
}