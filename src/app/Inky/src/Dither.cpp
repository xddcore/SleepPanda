#include "Dither.hpp"
#include <limits>
#include <stdexcept>
#include <map>

static const uint8_t ditherLut[] 
{
  // 0x00
  0, 0, 0, 0, 
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  // 0x10
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  1, 0, 0, 0,
  // 0x20
  0, 0, 0, 0, 
  0, 0, 1, 0,
  0, 0, 0, 0,
  1, 0, 0, 0,
  // 0x30
  0, 0, 0, 0, 
  1, 0, 1, 0,
  0, 0, 0, 0,
  1, 0, 0, 0,
  // 0x40
  0, 0, 0, 0,
  1, 0, 1, 0,
  0, 0, 0, 0,
  1, 0, 1, 0,
  // 0x50
  0, 0, 0, 0,
  1, 0, 1, 0,
  0, 1, 0, 0,
  1, 0, 1, 0,
  // 0x60
  0, 0, 0, 1,
  1, 0, 1, 0,
  0, 1, 0, 0,
  1, 0, 1, 0,
  // 0x70
  0, 0, 0, 1,
  1, 0, 1, 0,
  0, 1, 0, 1,
  1, 0, 1, 0,
  // 0x80
  0, 1, 0, 1,
  1, 0, 1, 0,
  0, 1, 0, 1,
  1, 0, 1, 0,
  // 0x90
  0, 1, 0, 1,
  1, 0, 1, 0,
  0, 1, 0, 1,
  1, 1, 1, 0,
  // 0xA0
  0, 1, 0, 1,
  1, 0, 1, 1,
  0, 1, 0, 1,
  1, 1, 1, 0,
  // 0xB0
  0, 1, 0, 1,
  1, 0, 1, 1,
  0, 1, 0, 1,
  1, 1, 1, 1,
  // 0xC0
  0, 1, 0, 1,
  1, 1, 1, 1,
  0, 1, 0, 1,
  1, 1, 1, 1,
  // 0xD0
  0, 1, 0, 1,
  1, 1, 1, 1,
  1, 1, 0, 1,
  1, 1, 1, 1,
  // 0xE0
  0, 1, 1, 1,
  1, 1, 1, 1,
  1, 1, 0, 1,
  1, 1, 1, 1,
  // 0xF0
  0, 1, 1, 1,
  1, 1, 1, 1,
  1, 1, 1, 1,
  1, 1, 1, 1,
  // 0x100
  1, 1, 1, 1,
  1, 1, 1, 1,
  1, 1, 1, 1,
  1, 1, 1, 1,
};

static void checkDitherSrcDest(const Image& sourceImage, Image& destImage)
{
  if (sourceImage.format() != PixelFormat::RGBA)
  {
    throw std::invalid_argument("Source image format must be RGBA");
  }

  if (destImage.format() != PixelFormat::IndexedColor)
  {
    throw std::invalid_argument("Dest image format must be an Indexed format!");
  }

  if (destImage.height() != sourceImage.height() || 
      destImage.width() != sourceImage.width())
  {
    throw std::invalid_argument("Dest image and source images must have the same dimensions!");
  }
}

void patternDither(const Image& sourceImage, Image& destImage)
{
  checkDitherSrcDest(sourceImage, destImage);

  int width = sourceImage.width();
  int height = sourceImage.height();
  const RGBAColor* dataRGBA = (const RGBAColor*)sourceImage.data();
  uint8_t* dataInky = destImage.data();

  IndexedColor black = destImage.colorMap().toIndexedColor(ColorName::Black);
  IndexedColor white = destImage.colorMap().toIndexedColor(ColorName::White);

  // Iterate over all the color data, just converting to black and white
  for (int y=0; y < height; ++y)
  {
    for (int x=0; x < width; ++x)
    {
      int lutOffset = ((int)dataRGBA->getGrayValue() + 0x08) & 0x1F0;
      if (ditherLut[lutOffset + (y%4)*4+(x%4)])
      {
        (*dataInky) = white;
      }
      else
      {
        (*dataInky) = black;
      }
      ++dataInky;
      ++dataRGBA;
    }
  }
}

static std::vector<LabColor> getLabVector(const Image& sourceImage, PixelFormat destFormat)
{
  if (sourceImage.format() != PixelFormat::RGBA)
  {
    throw std::invalid_argument("Source image format must be RGBA");
  }

  if (destFormat != PixelFormat::IndexedColor)
  {
    throw std::invalid_argument("Dest image format must be an indexed format!");
  }

  // Copy the source image to a float array for convenience
  // (the algorithm later will want to treat the values as floats)
  std::vector<LabColor> labVals(sourceImage.width()*sourceImage.height());
  
  const RGBAColor* dataRGBA = (const RGBAColor*)sourceImage.data();
  // Yellow and red accents work best right on the color image
  for (int i=0; i < labVals.size(); ++i)
  {
    labVals[i] = dataRGBA[i].toLab();
  }
  
  return labVals;
}

void diffuseError_dk(std::vector<LabColor>& labVals, const LabColor& oldValue, const LabColor& error, const int x, const int y, const int width, const int height)
{
  if (x < width-1)                                           // Traditional weights...
    labVals[ (x+1)+(y)*width ] += error *   (5.0f / 16.0f);  // 7
  if (x > 0 && y < height-1)
    labVals[ (x-1)+(y+1)*width ] += error * (3.0f / 16.0f);  // 3
  if (y < height-1)
    labVals[ (x)+(y+1)*width ] += error *   (5.0f / 16.0f);  // 5
  if (x < width-1 && y < height-1)
    labVals[ (x+1)+(y+1)*width ] += error * (3.0f / 16.0f);  // 1
}

void diffuseError(std::vector<LabColor>& labVals, const LabColor& oldValue, const LabColor& error, const int x, const int y, const int width, const int height)
{
  if (x < width-1)
    labVals[ (x+1)+(y)*width ] += error *   (7.0f / 16.0f);  
  if (x > 0 && y < height-1)
    labVals[ (x-1)+(y+1)*width ] += error * (3.0f / 16.0f);  
  if (y < height-1)
    labVals[ (x)+(y+1)*width ] += error *   (5.0f / 16.0f);  
  if (x < width-1 && y < height-1)
    labVals[ (x+1)+(y+1)*width ] += error * (1.0f / 16.0f);  
}

template <typename T> 
T sign(T val) 
{
    return (T(0) < val) - (val < T(0));
}

void diffuseErrorGradientInd(std::vector<LabColor>& labVals, const LabColor& oldValue, const LabColor& error, const int x, const int y, const int width, const int height)
{
  //static const float weights[] = {(1.0f / 16.0f), (3.0f / 16.0f), (5.0f / 16.0f), (7.0f / 16.0f)};
  static const float weights[] = {(7.0f / 16.0f), (5.0f / 16.0f), (3.0f / 16.0f), (1.0f / 16.0f)};


  if (x == width-1 || x == 0 || y == height-1 || y == 0)
  {
    diffuseError(labVals, oldValue, error, x, y, width, height);
  }
  else
  {
    // Fetch refs to the error candidates
    LabColor& labE = labVals[ (x+1)+(y)*width ];
    LabColor& labSW = labVals[ (x-1)+(y+1)*width ];
    LabColor& labS = labVals[ (x)+(y+1)*width ];
    LabColor& labSE = labVals[ (x+1)+(y+1)*width ];

    // Calculate gradient to the error candidates
    LabColor errorSign = {sign(error.L), sign(error.a), sign(error.b)};
    LabColor eE = (oldValue - labE) * errorSign;
    LabColor eSW = (oldValue - labSW) * errorSign;
    LabColor eS = (oldValue - labS) * errorSign;
    LabColor eSE = (oldValue - labSE) * errorSign;

    // Construct a sorted map for each channel
    std::multimap<float, LabColor*> sortedErrorsL
    {
      {eE.L, &labE},
      {eSW.L, &labSW},
      {eS.L, &labS},
      {eSE.L, &labSE}
    };
    int i = 0;
    for (auto& [mag, color] : sortedErrorsL)
    {
      color->L += weights[i] * error.L;
      ++i;
    }

    std::multimap<float, LabColor*> sortedErrorsA
    {
      {eE.a, &labE},
      {eSW.a, &labSW},
      {eS.a, &labS},
      {eSE.a, &labSE}
    };
    i = 0;
    for (auto& [mag, color] : sortedErrorsA)
    {
      color->a += weights[i] * error.a;
      ++i;
    }

    std::multimap<float, LabColor*> sortedErrorsB
    {
      {eE.b, &labE},
      {eSW.b, &labSW},
      {eS.b, &labS},
      {eSE.b, &labSE}
    };
    i = 0;
    for (auto& [mag, color] : sortedErrorsB)
    {
      color->b += weights[i] * error.b;
      ++i;
    }
  }
}

void diffuseErrorGradient(std::vector<LabColor>& labVals, const LabColor& oldValue, const LabColor& error, const int x, const int y, const int width, const int height)
{
  if (x == width-1 || x == 0 || y == height-1 || y == 0)
  {
    diffuseError(labVals, oldValue, error, x, y, width, height);
  }
  else
  {
    //static const float weights[] = {(1.0f / 16.0f), (3.0f / 16.0f), (5.0f / 16.0f), (7.0f / 16.0f)};
    static const float weights[] = {(8.0f / 16.0f), (5.0f / 16.0f), (3.0f / 16.0f), (1.0f / 16.0f)};

    // Fetch refs to the error candidates
    LabColor& labE = labVals[ (x+1)+(y)*width ];
    //LabColor& labSW = labVals[ (x-1)+(y+1)*width ];
    LabColor& labS = labVals[ (x)+(y+1)*width ];
    LabColor& labSE = labVals[ (x+1)+(y+1)*width ];

    // Calculate gradient to the error candidates
    LabColor errorSign = {sign(error.L), sign(error.a), sign(error.b)};
    LabColor eE = (oldValue - labE) * errorSign;
    //LabColor eSW = (oldValue - labSW) * errorSign;
    LabColor eS = (oldValue - labS) * errorSign;
    LabColor eSE = (oldValue - labSE) * errorSign;

    // Construct a sorted map for each channel
    std::multimap<float, LabColor*> sortedErrorsL
    {
      {eE.L  /*+ eE.a + eE.b  */, &labE},
      //{eSW.L /*+ eSW.a + eSW.b*/, &labSW},
      {eS.L  /*+ eS.a + eS.b  */, &labS},
      {eSE.L /*+ eSE.a + eSE.b*/, &labSE}
    };
    int i = 0;
    for (auto& [mag, color] : sortedErrorsL)
    {
      (*color) += error * weights[i];
      ++i;
    }
  }
}

void diffusionDither(const Image& sourceImage, Image& destImage, float ditherAccuracy)
{
  checkDitherSrcDest(sourceImage, destImage);

  int width = sourceImage.width();
  int height = sourceImage.height();
  PixelFormat format = destImage.format();
  
  // Get the source image data in LAB color format
  std::vector<LabColor> labVals = getLabVector(sourceImage, destImage.format());

  // Convert to Inky using Floyd-Steinberg dithering
  uint8_t* dataInky = destImage.data();
  const IndexedColorMap& colorMap = destImage.colorMap();
  LabColor error, oldValue;
  for (int y=0; y < height; ++y)
  {
    for (int x=0; x < width; ++x)
    {
      oldValue = labVals[x+y*width];
      (*dataInky) = colorMap.toIndexedColor(oldValue, error);
      error = error * ditherAccuracy;

      diffuseError(labVals, oldValue, error, x, y, width, height);
      
      ++dataInky;
    }
  }
}
