#pragma once

#include <stdint.h>
#include <unordered_map>
#include <vector>

enum class ColorName
{
  White,
  Red,
  Orange,
  Yellow,
  Green,
  Blue,
  Black,
  Clean
};

typedef uint8_t IndexedColor;
struct RGBAColor;
struct HSVColor;
struct LabColor;

#pragma pack(push, 1)
struct HSVColor
{
  float H = 0.0f;
  float S = 0.0f;
  float V = 0.0f;
  float A = 1.0f;

  RGBAColor toRGB();
};

struct RGBAColor
{
  uint8_t R = 0;
  uint8_t G = 0;
  uint8_t B = 0;
  uint8_t A = 255;

  HSVColor toHSV() const;
  LabColor toLab() const;
  uint8_t getBrightestChannel() const;
  uint8_t getDarkestChannel() const;
  uint8_t getGrayValue() const;
};

struct XYZColor
{
  float X = 0;
  float Y = 0;
  float Z = 0;
};

struct LabColor
{
  float L = 0;
  float a = 0;
  float b = 0;

  RGBAColor toRgba() const;

  LabColor operator+ (const LabColor& c)  const
  {
    return {L + c.L, a + c.a, b + c.b};
  }

  LabColor operator* (const LabColor& c)  const
  {
    return {L * c.L, a * c.a, b * c.b};
  }

  void operator+= (const LabColor& c)
  {
    L += c.L;
    a += c.a;
    b += c.b;
  }

  LabColor operator- (const LabColor& c) const
  {
    return {L - c.L, a - c.a, b - c.b};
  }

  LabColor operator* (float c) const
  {
    return {c*L, c*a, c*b};
  }

  friend LabColor operator*(float c, const LabColor& rhs)
  {
      return {c*rhs.L, c*rhs.a, c*rhs.b};
  }

  float deltaE(const LabColor& other) const;
};
#pragma pack(pop)

struct IndexedColorMap
{
  IndexedColorMap() = default;
  IndexedColorMap(std::vector<std::tuple<ColorName,IndexedColor,RGBAColor>> mapping);
  // Set the colors Black and White to (0,0,0) and (255,255,255) respectively, then rescale the rest
  void normalizePaletteByRgb(bool pinBlack = true, bool pinWhite = true);
  void normalizePaletteByLab(bool pinBlack = true, bool pinWhite = true);
  IndexedColor toIndexedColor(const LabColor& color, LabColor& error) const;
  IndexedColor toIndexedColor(const RGBAColor& color) const;
  IndexedColor toIndexedColor(const LabColor& color) const;
  IndexedColor toIndexedColor(const ColorName) const;
  RGBAColor toRGBAColor(const IndexedColor indexedColor) const;
  LabColor toLabColor(const IndexedColor indexedColor) const;
  uint8_t size() const;
  const std::vector<IndexedColor>& indexedColors() const;
  const std::vector<ColorName>& namedColors() const;
private:
  std::vector<IndexedColor> indexedColors_;
  std::vector<ColorName> namedColors_;
  std::unordered_map<IndexedColor,ColorName> indexToName;
  std::unordered_map<IndexedColor,RGBAColor> indexToRgba;
  std::unordered_map<IndexedColor,LabColor> indexToLab;
  std::unordered_map<ColorName,IndexedColor> nameToIndex;
  std::unordered_map<ColorName,RGBAColor> nameToRgba;
  std::unordered_map<ColorName,LabColor> nameToLab;
};