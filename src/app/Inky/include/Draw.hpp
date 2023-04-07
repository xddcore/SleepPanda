#pragma once

#include "Image.hpp"

namespace Draw
{
  enum class Font : int
  {
    Mono_4x6,
    Mono_6x6,
    Mono_8x12,
    Mono_32x48
  };

  enum class HAlign
  {
    Left,
    Center,
    Right
  };

  enum class VAlign
  {
    Top,
    Center,
    Bottom
  };

  struct TextStyle
  {
    HAlign hAlign = HAlign::Left;
    VAlign vAlign = VAlign::Top;
    RGBAColor color = {0,0,0,255};
    Font font = Font::Mono_8x12;
  };

  struct BoxStyle
  {
    HAlign hAlign = HAlign::Left;
    VAlign vAlign = VAlign::Top;
    RGBAColor color = {0,0,0,255};
  };

  void Text(Image& dest, int x, int y, const std::string& str, TextStyle style = {});

  void Box(Image& dest, int x, int y, int width, int height, BoxStyle style = {});
}