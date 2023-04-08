#include "Image.hpp"
#include "Dither.hpp"

#include <fmt/format.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <stdarg.h>
#include <cmath>

#include <base_resample.h>

Image::Image()
{
  width_ = 0;
  height_ = 0;
  format_ = PixelFormat::RGBA;
}

Image::Image(int width, int height)
{
  width_ = width;
  height_ = height;
  format_ = PixelFormat::RGBA;
  data_.resize(width_ * height_ * bytesPerPixel());
}

Image::Image(int width, int height, IndexedColorMap colorMap)
{
  width_ = width;
  height_ = height;
  format_ = PixelFormat::IndexedColor;
  colorMap_ = colorMap;
  data_.resize(width_ * height_ * bytesPerPixel());
}

int Image::bytesPerPixel() const
{
  return format_ == PixelFormat::RGBA ? 4 : 1;
}

void Image::toIndexed(IndexedColorMap colorMap, DitherSettings settings)
{
  toIndexed(*this, colorMap, settings);
}

void Image::toIndexed(Image &dest, IndexedColorMap colorMap, DitherSettings settings) const
{
  // Conversion type 1: RGBA to indexed
  if (format_ == PixelFormat::RGBA)
  {
    Image indexImage(width_, height_, colorMap);

    if (settings.ditherMode == DitherMode::Pattern)
    {
      patternDither(*this, indexImage);
    }
    else if (settings.ditherMode == DitherMode::Diffusion)
    {
      diffusionDither(*this, indexImage, settings.ditherAccuracy);
    }

    // Move the new image buffer into place
    dest.data_ = std::move(indexImage.data_);
    dest.width_ = width_;
    dest.height_ = height_;
    dest.format_ = PixelFormat::IndexedColor;
    dest.colorMap_ = colorMap;
    return;
  }
  // Conversion type 2: indexed to indexed (via RGBA)
  else
  {
    Image rgbaImage(width_, height_);
    toRGBA(rgbaImage);
    rgbaImage.toIndexed(dest, colorMap, settings);
    return;
  }
}

void Image::rotateFlip(FlipRotateOperation op)
{
  rotateFlip(*this, op);
}

void Image::rotateFlip(Image &dest, FlipRotateOperation op) const
{
  bool inPlace = (&dest == this);

  // No operation needed
  if (op == FlipRotateOperation::None)
  {
    if (!inPlace)
    {
      dest.data_ = data_;
      dest.width_ = width_;
      dest.height_ = height_;
    }
  }
  // We have to move the data
  else
  {
    std::vector<uint8_t> destData(data_.size());
    int bpp = bytesPerPixel();
    const uint8_t *src = data_.data();
    uint8_t *dst = destData.data();
    dest.width_ = width_;
    dest.height_ = height_;
    switch (op)
    {
    case FlipRotateOperation::Mirror:
      for (int y = 0; y < dest.height_; ++y)
      {
        for (int x = dest.width_ - 1; x >= 0; --x)
        {
          memcpy(dst + (x + y * dest.width_) * bpp, src, bpp);
          src += bpp;
        }
      }
      break;
    case FlipRotateOperation::Rotate180:
      for (int y = dest.height_ - 1; y >= 0; --y)
      {
        for (int x = dest.width_ - 1; x >= 0; --x)
        {
          memcpy(dst + (x + y * dest.width_) * bpp, src, bpp);
          src += bpp;
        }
      }
      break;

    case FlipRotateOperation::Rotate180Mirror:
      for (int y = dest.height_ - 1; y >= 0; --y)
      {
        for (int x = 0; x < dest.width_; ++x)
        {
          memcpy(dst + (x + y * dest.width_) * bpp, src, bpp);
          src += bpp;
        }
      }
      break;
    case FlipRotateOperation::Rotate90:
      std::swap(dest.width_, dest.height_);
      for (int x = dest.width_ - 1; x >= 0; --x)
      {
        for (int y = 0; y < dest.height_; ++y)
        {
          memcpy(dst + (x + y * dest.width_) * bpp, src, bpp);
          src += bpp;
        }
      }
      break;
    case FlipRotateOperation::Rotate90Mirror:
      std::swap(dest.width_, dest.height_);
      for (int x = 0; x < dest.width_; ++x)
      {
        for (int y = 0; y < dest.height_; ++y)
        {
          memcpy(dst + (x + y * dest.width_) * bpp, src, bpp);
          src += bpp;
        }
      }
      break;
    case FlipRotateOperation::Rotate270:
      std::swap(dest.width_, dest.height_);
      for (int x = 0; x < dest.width_; ++x)
      {
        for (int y = dest.height_ - 1; y >= 0; --y)
        {
          memcpy(dst + (x + y * dest.width_) * bpp, src, bpp);
          src += bpp;
        }
      }
      break;
    case FlipRotateOperation::Rotate270Mirror:
      std::swap(dest.width_, dest.height_);
      for (int x = dest.width_ - 1; x >= 0; --x)
      {
        for (int y = dest.height_ - 1; y >= 0; --y)
        {
          memcpy(dst + (x + y * dest.width_) * bpp, src, bpp);
          src += bpp;
        }
      }
      break;
    default:
      break;
    }
    dest.data_ = std::move(destData);
  }

  // Set the size and format on the destination image
  dest.format_ = format_;

  return;
}

void Image::toRGBA()
{
  toRGBA(*this);
}

void Image::toRGBA(Image &dest) const
{
  bool inPlace = (&dest == this);

  // Image is the correct format already!
  // Just copy the data as-is
  if (format_ == PixelFormat::RGBA)
  {
    if (!inPlace)
    {
      dest.data_ = data_;
    }
  }
  // Conversion type 2: BW/BWR/BWY to RGBA
  else
  {
    std::vector<uint8_t> dataRGBA(4 * width_ * height_);
    RGBAColor *rgbaPtr = (RGBAColor *)dataRGBA.data();
    IndexedColor *indexedPtr = (IndexedColor *)data_.data();
    for (int i = 0; i < data_.size(); ++i)
    {
      rgbaPtr[i] = colorMap_.toRGBAColor(indexedPtr[i]);
    }
    dest.data_ = std::move(dataRGBA);
  }

  // Set the size and format on the destination image
  dest.width_ = width_;
  dest.height_ = height_;
  dest.format_ = PixelFormat::RGBA;
  return;
}

void Image::scale(int width, int height, ScaleSettings settings)
{
  scale(*this, width, height, settings);
}

void Image::scale(Image &dest, int width, int height, ScaleSettings settings) const
{
  bool inPlace = (&dest == this);

  float xScale = (float)width / (float)width_;
  float yScale = (float)height / (float)height_;

  int uncroppedWidth = width;
  int uncroppedHeight = height;

  if (settings.scaleMode == ScaleMode::Fill)
  {
    float scale = std::max(xScale, yScale);
    uncroppedWidth = (int)((float)width_ * scale);
    uncroppedHeight = (int)((float)height_ * scale);
  }
  else if (settings.scaleMode == ScaleMode::Fit)
  {
    float scale = std::min(xScale, yScale);
    uncroppedWidth = (int)((float)width_ * scale);
    uncroppedHeight = (int)((float)height_ * scale);
  }

  if (settings.interpolationMode == InterpolationMode::Auto)
  {
    // Use Bilinear for enlargement and Gaussian for reduction
    settings.interpolationMode = (width > width_) ? InterpolationMode::Bilinear : InterpolationMode::Gaussian;
  }

  if (width == width_ && height == height_)
  {
    // Image is the correct size already!
    // Just copy the data as-is
    if (!inPlace)
    {
      dest.data_ = data_;
    }
  }
  else if (format_ == PixelFormat::RGBA)
  {
    std::vector<uint8_t> scaleData(uncroppedWidth * uncroppedHeight * 4);
    base::ResampleImage<4>(data_.data(), (uint32_t)width_, (uint32_t)height_,
                           scaleData.data(), (uint32_t)uncroppedWidth, (uint32_t)uncroppedHeight,
                           (base::KernelType)settings.interpolationMode);
    dest.data_ = std::move(scaleData);
  }
  else
  {
    std::vector<uint8_t> scaleData(uncroppedWidth * uncroppedHeight);
    base::ResampleImage<1>(data_.data(), (uint32_t)width_, (uint32_t)height_,
                           scaleData.data(), (uint32_t)uncroppedWidth, (uint32_t)uncroppedHeight,
                           (base::KernelType)settings.interpolationMode);
    dest.data_ = std::move(scaleData);
  }

  dest.width_ = uncroppedWidth;
  dest.height_ = uncroppedHeight;
  dest.format_ = format_;

  if (width != uncroppedWidth || height != uncroppedHeight)
  {
    dest.crop((uncroppedWidth - width) / 2, (uncroppedHeight - height) / 2, width, height, settings);
  }
}

void Image::crop(int x, int y, int width, int height, ScaleSettings settings)
{
  crop(*this, x, y, width, height, settings);
}

void Image::crop(Image &dest, int x, int y, int width, int height, ScaleSettings settings) const
{
  bool inPlace = (&dest == this);

  if (x == 0 && y == 0 && width == width_ && height_ == height)
  {
    if (!inPlace)
    {
      dest.width_ = width_;
      dest.height_ = height_;
      dest.format_ = format_;
      dest.data_ = data_;
    }
    return;
  }

  // Create a buffer for the cropped image
  std::vector<uint8_t> croppedData(width * height * bytesPerPixel());
  int size = width * height;

  // Fill in the background color
  if (format_ == PixelFormat::RGBA)
  {
    RGBAColor *data = (RGBAColor *)croppedData.data();
    for (int i = 0; i < size; ++i)
    {
      data[i] = settings.backgroundColor;
    }
  }
  else
  {
    IndexedColor backgroundColor = colorMap_.toIndexedColor(settings.backgroundColor);
    IndexedColor *data = (IndexedColor *)croppedData.data();
    for (int i = 0; i < size; ++i)
    {
      data[i] = backgroundColor;
    }
  }

  // Figure out the copy boundaries
  int srcX = (x < 0) ? 0 : x;
  int srcY = (y < 0) ? 0 : y;
  int dstX = (x > 0) ? 0 : -x;
  int dstY = (y > 0) ? 0 : -y;
  int cpyWidth = std::min(width - dstX, width_ - srcX);
  int cpyHeight = std::min(height - dstY, height_ - srcY);

  // Do the copy row by row
  for (int row = 0; row < cpyHeight; ++row)
  {
    const uint8_t *src = data_.data() + (srcX + (srcY + row) * width_) * bytesPerPixel();
    uint8_t *dst = croppedData.data() + (dstX + (dstY + row) * width) * bytesPerPixel();
    memcpy(dst, src, cpyWidth * bytesPerPixel());
  }

  // Move the cropped data into place
  dest.width_ = width;
  dest.height_ = height;
  dest.format_ = format_;
  dest.data_ = std::move(croppedData);
}

const IndexedColorMap &Image::colorMap() const
{
  return colorMap_;
}

uint8_t *Image::data()
{
  return data_.data();
}

const uint8_t *Image::data() const
{
  return data_.data();
}

int Image::width() const
{
  return width_;
}

int Image::height() const
{
  return height_;
}

BoundingBox Image::bounds() const
{
  return {0, 0, width_, height_};
}

PixelFormat Image::format() const
{
  return format_;
}