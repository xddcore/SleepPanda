#pragma once

#include "Image.hpp"
#include <filesystem>
#include <iostream>

enum class ImageFormat
{
  Auto,
  PNG,
  JPEG
};

struct ImageLoadSettings
{
  // Use any orientation metadata to rectify the image as it is loaded
  bool autoRotate = true;
};

struct ImageSaveSettings
{
  ImageFormat saveFormat = ImageFormat::Auto;
  int jpegQuality = 75;
};

struct ImageIO
{
public:
  ImageIO() = delete;
  static Image LoadFromStream(std::istream&, ImageLoadSettings settings = {});
  static Image LoadFromBuffer(const std::string&, ImageLoadSettings settings = {});
  static Image LoadFromFile(std::filesystem::path, ImageLoadSettings settings = {});
  static void SaveToStream(const Image&, std::ostream&, ImageSaveSettings settings = {});
  static void SaveToBuffer(const Image&, std::string&, ImageSaveSettings settings = {});
  static void SaveToFile(std::filesystem::path, const Image&, ImageSaveSettings settings = {});
private: 
  static void readJpeg(std::istream&, Image&, ImageLoadSettings);
  static void writeJpeg(std::ostream&, const Image&, ImageSaveSettings);
  static void readPng(std::istream&, Image&, ImageLoadSettings);
  static void writePng(std::ostream&, const Image&, ImageSaveSettings);
};