#include "Color.hpp"

#include <cmath>

#include <algorithm>
#include <random>
#include <stdexcept>

IndexedColorMap::IndexedColorMap(std::vector<std::tuple<ColorName,IndexedColor,RGBAColor>> mapping)
{
  if (mapping.size() > 254)
  {
    throw std::invalid_argument("Cannot create IndexedColorMap with more than 254 mappings!");
  }

  // Create exhaustive mappings
  for (const auto& [name, index, rgba] : mapping)
  {
    indexedColors_.push_back(index);
    namedColors_.push_back(name);
    indexToName[index] = name;
    indexToRgba[index] = rgba;
    indexToLab[index] = rgba.toLab();
    nameToIndex[name] = index;
    nameToRgba[name] = rgba;
    nameToLab[name] = rgba.toLab();
  }
}

template <typename T>
static T remap(T value, T inMin, T inMax, T outMin, T outMax)
{
  double t = (double)(value - inMin)/(double)(inMax-inMin);
  double out = t * (double)(outMax-outMin) + (double)(outMin);
  return std::clamp((T)std::clamp(out, (double)outMin, (double)outMax), outMin, outMax);
}

void IndexedColorMap::normalizePaletteByRgb(bool pinBlack, bool pinWhite)
{
  auto max = pinWhite ? toRGBAColor(toIndexedColor(ColorName::White)).getBrightestChannel() : (uint8_t)255;
  auto min = pinBlack ? toRGBAColor(toIndexedColor(ColorName::Black)).getDarkestChannel() : (uint8_t)0;

  for (auto index : indexedColors_)
  {
    auto name = indexToName[index];
    RGBAColor colorRgb = indexToRgba[index];
    colorRgb.R = remap(colorRgb.R, min, max, (uint8_t)0, (uint8_t)255);
    colorRgb.G = remap(colorRgb.G, min, max, (uint8_t)0, (uint8_t)255);
    colorRgb.B = remap(colorRgb.B, min, max, (uint8_t)0, (uint8_t)255);
    LabColor colorLab = colorRgb.toLab();

    // Be sure to update all the following indices
    // indexToRgba, indexToLab, nameToRgba, nameToLab
    indexToRgba[index] = colorRgb;
    nameToRgba[name] = colorRgb;
    indexToLab[index] = colorLab;
    nameToLab[name] = colorLab;
  }
}

void IndexedColorMap::normalizePaletteByLab(bool pinBlack, bool pinWhite)
{
  float max = pinWhite ? toLabColor(toIndexedColor(ColorName::White)).L : 100.0f;
  float min = pinBlack ? toLabColor(toIndexedColor(ColorName::Black)).L : 0.0f;

  for (auto index : indexedColors_)
  {
    auto name = indexToName[index];
    LabColor colorLab = indexToLab[index];
    colorLab.L = remap(colorLab.L, min, max, 0.0f, 100.0f);
    RGBAColor colorRgb = colorLab.toRgba();

    // Be sure to update all the following indices
    // indexToRgba, indexToLab, nameToRgba, nameToLab
    indexToRgba[index] = colorRgb;
    nameToRgba[name] = colorRgb;
    indexToLab[index] = colorLab;
    nameToLab[name] = colorLab;
  }
}

const std::vector<IndexedColor>& IndexedColorMap::indexedColors() const 
{
  return indexedColors_;
}

const std::vector<ColorName>& IndexedColorMap::namedColors() const 
{
  return namedColors_;
}

IndexedColor IndexedColorMap::toIndexedColor(const LabColor& color, LabColor& error) const
{
  // Find the indexed color with the minimum deltaE from the specified color
  float minDeltaE = std::numeric_limits<float>::infinity();
  IndexedColor minIndexColor = 0;
  LabColor minLabColor;
  for (const auto& [indexedColor, refColor] : indexToLab)
  {
    float dE = refColor.deltaE(color);
    if (dE < minDeltaE)
    {
      minDeltaE = dE;
      minIndexColor = indexedColor;
      minLabColor = refColor;
    }
  }
  error = color - minLabColor;
  return minIndexColor;
}

IndexedColor IndexedColorMap::toIndexedColor(const RGBAColor& color) const
{
  LabColor error;
  return toIndexedColor(color.toLab(), error);
}

IndexedColor IndexedColorMap::toIndexedColor(const LabColor& color) const
{
  LabColor error;
  return toIndexedColor(color, error);
}

RGBAColor IndexedColorMap::toRGBAColor(const IndexedColor indexedColor) const
{
  if (indexToRgba.count(indexedColor) > 0)
    return indexToRgba.at(indexedColor);
  return RGBAColor();
}

LabColor IndexedColorMap::toLabColor(const IndexedColor indexedColor) const
{
  if (indexToLab.count(indexedColor) > 0)
    return indexToLab.at(indexedColor);
  return LabColor();
}

IndexedColor IndexedColorMap::toIndexedColor(const ColorName namedColor) const
{
  if (nameToIndex.count(namedColor) > 0)
    return nameToIndex.at(namedColor);
  return 255;
}

uint8_t IndexedColorMap::size() const
{
  return (uint8_t) indexedColors_.size();
}

RGBAColor HSVColor::toRGB()
{
  float r, g, b;
  int range = (int)std::floor(H / 60.0f);
  float c = V * S;
  float x = c * (1 - std::abs(fmod(H / 60.0f, 2.0f) - 1));
  float m = V - c;

  switch (range)
  {
  case 0:
    r = (c + m);
    g = (x + m);
    b = m;
    break;
  case 1:
    r = (x + m);
    g = (c + m);
    b = m;
    break;
  case 2:
    r = m;
    g = (c + m);
    b = (x + m);
    break;
  case 3:
    r = m;
    g = (x + m);
    b = (c + m);
    break;
  case 4:
    r = (x + m);
    g = m;
    b = (c + m);
    break;
  default: // case 5:
    r = (c + m);
    g = m;
    b = (x + m);
    break;
  }

  return {
      (uint8_t)std::clamp((r * 255.0f), 0.0f, 255.0f),
      (uint8_t)std::clamp((g * 255.0f), 0.0f, 255.0f),
      (uint8_t)std::clamp((b * 255.0f), 0.0f, 255.0f),
      (uint8_t)std::clamp((A * 255.0f), 0.0f, 255.0f)};
}

HSVColor RGBAColor::toHSV() const
{
  HSVColor hsv;

  float r = std::clamp(R / 255.0f, 0.0f, 1.0f);
  float g = std::clamp(G / 255.0f, 0.0f, 1.0f);
  float b = std::clamp(B / 255.0f, 0.0f, 1.0f);
  hsv.A = std::clamp(A / 255.0f, 0.0f, 1.0f);

  float min = std::min(r, std::min(g, b));
  float max = std::max(r, std::max(g, b));
  float delta = max - min;

  hsv.V = max;
  hsv.S = (max > 1e-3) ? (delta / max) : 0;

  if (delta == 0)
  {
    hsv.H = 0;
  }
  else
  {
    if (r == max)
      hsv.H = (g - b) / delta;
    else if (g == max)
      hsv.H = 2 + (b - r) / delta;
    else if (b == max)
      hsv.H = 4 + (r - g) / delta;

    hsv.H *= 60;
    hsv.H = fmod(hsv.H + 360, 360);
  }
  return hsv;
}

uint8_t RGBAColor::getDarkestChannel() const
{
  return std::min({R, G, B});
}

uint8_t RGBAColor::getBrightestChannel() const
{
  return std::max({R, G, B});
}


uint8_t RGBAColor::getGrayValue() const
{
  return (uint8_t)(0.299f * (float)R + 0.587f * (float)G + 0.114f * (float)B);
}

void rgbToXyz(const RGBAColor &rgb, XYZColor &xyz)
{
  double r = (float)rgb.R / 255.0;
  double g = (float)rgb.G / 255.0;
  double b = (float)rgb.B / 255.0;

  r = ((r > 0.04045) ? pow((r + 0.055) / 1.055, 2.4) : (r / 12.92)) * 100.0;
  g = ((g > 0.04045) ? pow((g + 0.055) / 1.055, 2.4) : (g / 12.92)) * 100.0;
  b = ((b > 0.04045) ? pow((b + 0.055) / 1.055, 2.4) : (b / 12.92)) * 100.0;

  xyz.X = r * 0.4124564 + g * 0.3575761 + b * 0.1804375;
  xyz.Y = r * 0.2126729 + g * 0.7151522 + b * 0.0721750;
  xyz.Z = r * 0.0193339 + g * 0.1191920 + b * 0.9503041;
}

void xyzToRgb(const XYZColor &xyz, RGBAColor &rgb)
{
  double x = xyz.X / 100.0;
  double y = xyz.Y / 100.0;
  double z = xyz.Z / 100.0;

  double r = x * 3.2404542 + y * -1.5371385 + z * -0.4985314;
  double g = x * -0.9692660 + y * 1.8760108 + z * 0.0415560;
  double b = x * 0.0556434 + y * -0.2040259 + z * 1.0572252;

  r = ((r > 0.0031308) ? (1.055 * pow(r, 1 / 2.4) - 0.055) : (12.92 * r)) * 255.0;
  g = ((g > 0.0031308) ? (1.055 * pow(g, 1 / 2.4) - 0.055) : (12.92 * g)) * 255.0;
  b = ((b > 0.0031308) ? (1.055 * pow(b, 1 / 2.4) - 0.055) : (12.92 * b)) * 255.0;

  rgb.R = r;
  rgb.G = g;
  rgb.B = b;
  rgb.A = 255;
}

static void rgbToLab(const RGBAColor &rgb, LabColor &lab)
{
  XYZColor xyz;
  rgbToXyz(rgb, xyz);

  double x = xyz.X / 95.047;
  double y = xyz.Y / 100.00;
  double z = xyz.Z / 108.883;

  x = (x > 0.008856) ? cbrt(x) : (7.787 * x + 16.0 / 116.0);
  y = (y > 0.008856) ? cbrt(y) : (7.787 * y + 16.0 / 116.0);
  z = (z > 0.008856) ? cbrt(z) : (7.787 * z + 16.0 / 116.0);

  lab.L = (116.0 * y) - 16;
  lab.a = 500 * (x - y);
  lab.b = 200 * (y - z);
}

static void labToRgb(const LabColor &lab, RGBAColor &rgb)
{
  double y = (lab.L + 16.0) / 116.0;
  double x = lab.a / 500.0 + y;
  double z = y - lab.b / 200.0;

  double x3 = std::pow(x, 3);
  double y3 = std::pow(y, 3);
  double z3 = std::pow(z, 3);

  x = ((x3 > 0.008856) ? x3 : ((x - 16.0 / 116.0) / 7.787)) * 95.047;
  y = ((y3 > 0.008856) ? y3 : ((y - 16.0 / 116.0) / 7.787)) * 100.0;
  z = ((z3 > 0.008856) ? z3 : ((z - 16.0 / 116.0) / 7.787)) * 108.883;

  xyzToRgb({(float)x, (float)y, (float)z}, rgb);
}

LabColor RGBAColor::toLab() const
{
  LabColor lab;
  rgbToLab(*this, lab);
  return lab;
}

RGBAColor LabColor::toRgba() const
{
  RGBAColor rgb;
  labToRgb(*this, rgb);
  return rgb;
}

float LabColor::deltaE(const LabColor& other) const
{
  return sqrtf(powf(L-other.L, 2) + powf(a-other.a, 2) + powf(b-other.b, 2));
}