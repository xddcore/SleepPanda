#include "Draw.hpp"
#include "Dither.hpp"
#include <unordered_map>
#include <ImageIO.hpp>

namespace Draw
{

static Image loadFont(const std::string& path)
{
  static IndexedColorMap colorMap(
  {
        {ColorName::Black, 0, {0,0,0}},
        {ColorName::White, 1, {255,255,255}}
  });
  Image font = ImageIO::LoadFromFile(path);
  // Create a binarized index image of the font
  font.toIndexed(colorMap, {.ditherAccuracy = 0.0f});
  return font;
}

// Static resoures for drawing text on to images
const static std::unordered_map<Font, Image> Fonts
{
  {Font::Mono_4x6,  loadFont("resources/font_4x6.png")},
  {Font::Mono_6x6,  loadFont("resources/font_6x6.png")},
  {Font::Mono_8x12, loadFont("resources/font_8x12.png")},
  {Font::Mono_32x48, loadFont("resources/font_32x48.png")},
};

template <typename PixelType>
static inline void characterBlit(const char character, const int x, const int y, const PixelType& color,
                                 const IndexedColor* fontData, const int charWidth, const int charHeight, const int fontWidth,
                                 PixelType* destData, BoundingBox& destBox)
{
  // Create a bounding box for the character we want to blit
  BoundingBox charBox {0, 0, charWidth, charHeight};
  int charOffsetX = (character % 16) * charWidth;
  int charOffsetY = (character / 16) * charHeight;

  // Move the dest box so that the charBox is in the right place on the image
  destBox.x = -x;
  destBox.y = -y;

  // Clip the char box to the dest box
  charBox.clipTo(destBox);

  // Check if there's any blittable area left
  if (charBox.width > 0 && charBox.height > 0)
  {
    // Now it should be safe to blindly blit the charBox on to destData
    for (int iY = 0; iY < charBox.height; ++iY)
    {
      for (int iX = 0; iX < charBox.width; ++iX)
      {
        if (fontData[iX+charOffsetX+charBox.x+(iY+charOffsetY+charBox.y)*fontWidth] != 0)
        {
          destData[iX+charBox.x+x+(iY+charBox.y+y)*destBox.width] = color;
        }
      }
    }
  }
}

void Text(Image& dest, int x, int y, const std::string& str, TextStyle style)
{
  const Image& font = Fonts.at(style.font);
  int charWidth = font.width() / 16;
  int charHeight = font.height() / 16;

  if (style.hAlign == HAlign::Center)
  {
    x -= ((str.size() * charWidth) / 2);
  }
  else if (style.hAlign == HAlign::Right)
  {
    x -= (str.size() * charWidth);
  }

  if (style.vAlign == VAlign::Center)
  {
    y -= (charHeight / 2);
  }
  else if (style.vAlign == VAlign::Bottom)
  {
    y -= charHeight;
  }

  if (dest.format() == PixelFormat::RGBA)
  {
    RGBAColor* destData = (RGBAColor*)dest.data();
    BoundingBox destBox = dest.bounds();
    for (const auto& ch : str)
    {
      characterBlit<RGBAColor>(ch, x, y, style.color,
                                 (IndexedColor*)font.data(), charWidth, charHeight, font.width(),
                                 destData, destBox);
      x += charWidth;
    }
  }
  else
  {
    IndexedColor* destData = (IndexedColor*)dest.data();
    IndexedColor color = dest.colorMap().toIndexedColor(style.color);
    BoundingBox destBox = dest.bounds();
    for (const auto& ch : str)
    {
      characterBlit<IndexedColor>(ch, x, y, color,
                                (IndexedColor*)font.data(), charWidth, charHeight, font.width(),
                                destData, destBox);
      x += charWidth;
    }
  }
}

template <typename PixelType>
static inline void fill(BoundingBox fillArea, const PixelType& color, PixelType* destData, const BoundingBox& destBox)
{
  // Clip the char box to the dest box
  fillArea.clipTo(destBox);

  // Check if there's any blittable area left
  if (fillArea.width > 0 && fillArea.height > 0)
  {
    // Now it should be safe to blindly blit the charBox on to destData
    for (int iY = 0; iY < fillArea.height; ++iY)
    {
      for (int iX = 0; iX < fillArea.width; ++iX)
      {
        //if ()
        //{
          destData[iX+fillArea.x+(iY+fillArea.y)*destBox.width] = color;
        //}
      }
    }
  }
}

void Box(Image& dest, int x, int y, int width, int height, BoxStyle style)
{
  if (style.hAlign == HAlign::Center)
  {
    x -= (width / 2);
  }
  else if (style.hAlign == HAlign::Right)
  {
    x -= width;
  }

  if (style.vAlign == VAlign::Center)
  {
    y -= (height / 2);
  }
  else if (style.vAlign == VAlign::Bottom)
  {
    y -= height;
  }

  if (dest.format() == PixelFormat::RGBA)
  {
    fill<RGBAColor>({x,y,width,height}, style.color, (RGBAColor*)dest.data(), dest.bounds());
  }
  else
  {
    IndexedColor color = dest.colorMap().toIndexedColor(style.color);
    fill<IndexedColor>({x,y,width,height}, color, (IndexedColor*)dest.data(), dest.bounds());
  }
}

}