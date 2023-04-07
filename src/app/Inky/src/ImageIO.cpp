// Hack to fix intellisense with some arm NEON code
#if __INTELLISENSE__
#undef __ARM_NEON
#undef __ARM_NEON__
#endif

#include "ImageIO.hpp"

#include <fmt/format.h>
#include <png.h>
#include <turbojpeg.h>
#include <TinyEXIF.h>

#include <fstream>
#include <sstream>

struct PngReadContext
{
  png_structp structp = nullptr;
  png_infop infop = nullptr;

  PngReadContext()
  {
    structp = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, 
      [](png_structp png_ptr, png_const_charp error_msg)
      {
        throw std::runtime_error(std::string(error_msg));
      }, NULL);
    if (structp != nullptr)
    {
      infop = png_create_info_struct(structp);
    }
  }
  ~PngReadContext()
  {
    png_destroy_read_struct(&structp, &infop, nullptr);
  }
  bool ok()
  {
    return (structp != nullptr) && (infop != nullptr);
  }
};

struct PngWriteContext
{
  png_structp structp = nullptr;
  png_infop infop = nullptr;

  PngWriteContext()
  {
    structp = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL,
      [](png_structp png_ptr, png_const_charp error_msg)
      {
        throw std::runtime_error(std::string(error_msg));
      }, NULL);
    if (structp != nullptr)
    {
      infop = png_create_info_struct(structp);
    }
  }
  ~PngWriteContext()
  {
    png_destroy_write_struct(&structp, &infop);
  }
  bool ok()
  {
    return (structp != nullptr) && (infop != nullptr);
  }
};

static ImageFormat detectFormat(uint8_t header[8])
{
  if (header[0] == 0xFF && 
      header[1] == 0xD8)
  {
    return ImageFormat::JPEG;
  }
  else if (header[0] == 0x89 &&
           header[1] == 0x50 &&
           header[2] == 0x4E &&
           header[3] == 0x47 &&
           header[4] == 0x0D &&
           header[5] == 0x0A &&
           header[6] == 0x1A &&
           header[7] == 0x0A)
  {
    return ImageFormat::PNG;
  }
  else
  {
    throw std::runtime_error("Unsupported image data! Only png and jpeg are supported.");
  }
}

static ImageFormat detectFormat(std::filesystem::path imagePath)
{
  std::string ext = imagePath.extension();
  std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

  if (ext == ".png")
  {
    return ImageFormat::PNG;
  }
  else if (ext == ".jpg" || ext == ".jpeg" || ext == ".jpe")
  {
    return ImageFormat::JPEG;
  }
  else
  {
    throw std::runtime_error(fmt::format("Unsupported extension '{}'! Only png and jpeg are supported.", ext));
  }
}

void ImageIO::readJpeg(std::istream& inputStream, Image& img, ImageLoadSettings settings)
{
  std::vector<char> compressedImage;
  compressedImage.assign(std::istreambuf_iterator<char>(inputStream), std::istreambuf_iterator<char>());

  int jpegSubsamp;
  tjhandle _jpegDecompressor = tjInitDecompress();
  // const cast to deal with C api
  tjDecompressHeader2(_jpegDecompressor, (uint8_t*)compressedImage.data(), compressedImage.size(), &img.width_, &img.height_, &jpegSubsamp);
  img.format_ = PixelFormat::RGBA;
  img.data_.resize(img.width_ * img.height_ * 4);
  tjDecompress2(_jpegDecompressor, (uint8_t*)compressedImage.data(), compressedImage.size(), img.data_.data(), img.width_, 0 /*pitch*/, img.height_, TJPF_RGBA, TJFLAG_FASTDCT);
  tjDestroy(_jpegDecompressor);

  if (settings.autoRotate)
  {
    TinyEXIF::EXIFInfo exif((uint8_t*)compressedImage.data(), compressedImage.size());
    img.rotateFlip((FlipRotateOperation)exif.Orientation);
  }
}

void ImageIO::writeJpeg(std::ostream& outputStream, const Image& img, ImageSaveSettings settings)
{
  long unsigned int jpegSize = 0;
  uint8_t *compressedImage = NULL; //!< Memory is allocated by tjCompress2 if _jpegSize == 0

  tjhandle jpegCompressor = tjInitCompress();

  tjCompress2(jpegCompressor, img.data(), img.width(), 0, img.height(), TJPF_RGBA,
              &compressedImage, &jpegSize, TJSAMP_444, settings.jpegQuality,
              TJFLAG_FASTDCT);

  tjDestroy(jpegCompressor);

  // Save the buffer to a file
  try
  {
    outputStream.write((char *)compressedImage, jpegSize);
  }
  catch (...)
  {
    tjFree(compressedImage);
    std::rethrow_exception(std::current_exception());
  }
  tjFree(compressedImage);
}

void ImageIO::readPng(std::istream& inputStream, Image& img, ImageLoadSettings /*settings*/)
{
  PngReadContext ctx;
  if (!ctx.ok())
  {
    throw std::runtime_error("Could not create png read context!");
  }

  png_set_read_fn(ctx.structp, &inputStream, 
  [](png_structp pngPtr, png_bytep outBytes, png_size_t byteCountToRead)
  {
    std::istream& inputStream = *(std::istream*)png_get_io_ptr(pngPtr);
    inputStream.read((char*)outBytes,(size_t)byteCountToRead);
  });
  
  png_read_info(ctx.structp, ctx.infop);
  img.width_ = png_get_image_width(ctx.structp, ctx.infop);
  img.height_ = png_get_image_height(ctx.structp, ctx.infop);
  uint8_t color_type = png_get_color_type(ctx.structp, ctx.infop);
  uint8_t bit_depth = png_get_bit_depth(ctx.structp, ctx.infop);
  int number_of_passes = png_set_interlace_handling(ctx.structp);
  png_read_update_info(ctx.structp, ctx.infop);

  // Size dest appropriately
  int strideInBytes = png_get_rowbytes(ctx.structp, ctx.infop);
  img.data_.resize(img.height_ * strideInBytes);

  std::vector<png_bytep> row_pointers(img.height_);
  for (int y = 0; y < img.height_; y++)
  {
    row_pointers[y] = img.data_.data() + strideInBytes * y;
  }

  png_read_image(ctx.structp, (png_bytepp)row_pointers.data());

  // It is possible that the image was read in as RGB instead of RGBA. Let's fix that here.
  if (strideInBytes == img.width_ * 3)
  {
    std::vector<uint8_t> rgbaVec(img.height_ * img.width_ * img.bytesPerPixel());
    uint8_t *rgbData = img.data_.data();
    uint8_t *rgbaData = rgbaVec.data();
    int pixels = img.width_ * img.height_;
    for (int p = 0; p < pixels; p++)
    {
      rgbaData[0] = rgbData[0];
      rgbaData[1] = rgbData[1];
      rgbaData[2] = rgbData[2];
      rgbaData[3] = 255;
      rgbaData += 4;
      rgbData += 3;
    }
    img.data_ = std::move(rgbaVec);
  }
  else if (strideInBytes != img.width_ * img.bytesPerPixel())
  {
    throw std::runtime_error("PNG read in unsupported color format! Only RGB and RGBA are supported.");
  }
}

void ImageIO::writePng(std::ostream& outputStream, const Image& img, ImageSaveSettings /*settings*/)
{
  PngWriteContext ctx;
  if (!ctx.ok())
  {
    throw std::runtime_error("Could not create png write context!");
  }

  png_set_IHDR(ctx.structp, ctx.infop, img.width_, img.height_, 8,
                PNG_COLOR_TYPE_RGBA,
                PNG_INTERLACE_NONE,
                PNG_COMPRESSION_TYPE_DEFAULT,
                PNG_FILTER_TYPE_DEFAULT);
  // png_set_compression_level(p, 1);
  std::vector<const uint8_t*> rows(img.height_);
  for (size_t y = 0; y < img.height_; ++y)
  {
    rows[y] = img.data_.data() + y * img.width_ * 4;
  }

  png_set_rows(ctx.structp, ctx.infop, const_cast<uint8_t**>(&rows[0]));
  png_set_write_fn(ctx.structp, &outputStream, [](png_structp png_ptr, png_bytep data, png_size_t length)
  {
    std::ostream& stream = *(std::ostream*)png_get_io_ptr(png_ptr);
    stream.write((char*)data, length);
  }, NULL);
  png_write_png(ctx.structp, ctx.infop, PNG_TRANSFORM_IDENTITY, NULL);
}

Image ImageIO::LoadFromStream(std::istream& stream, ImageLoadSettings settings)
{
  // Detect the file type
  uint8_t header[8];
  stream.read((char*)header, 8);
  auto format = detectFormat(header);

  // Rewind the input stream...
  stream.clear();
  stream.seekg(0, std::ios::beg);

  Image img;

  if (format == ImageFormat::JPEG)
  {
    readJpeg(stream, img, settings);
  }
  else if (format == ImageFormat::PNG)
  {
    readPng(stream, img, settings);
  }
  else
  {
    throw std::runtime_error("Unsupported image data!");
  }

  return img;
}

Image ImageIO::LoadFromBuffer(const std::string& str, ImageLoadSettings settings)
{
  std::istringstream inputStream(str);
  return LoadFromStream(inputStream, settings);
}

Image ImageIO::LoadFromFile(std::filesystem::path imagePath, ImageLoadSettings settings)
{
  std::ifstream inputStream(imagePath, std::ios::binary);
  return LoadFromStream(inputStream, settings);
}

void ImageIO::SaveToStream(const Image& image, std::ostream& stream, ImageSaveSettings settings)
{
  // At this point we have no context for Auto, so
  // just pick PNG
  if (settings.saveFormat == ImageFormat::Auto)
  {
    settings.saveFormat = ImageFormat::PNG;
  }

  // Verify the image is valid
  if (image.width() < 1 || image.height() < 1)
  {
    throw std::runtime_error("Cannot save zero-dimension image!");
  }

  // Convert to RGB if necessary
  Image rgba;
  const Image* imgToSave = &image;
  if (image.format() != PixelFormat::RGBA)
  {
    image.toRGBA(rgba);
    imgToSave = &rgba;
  }

  if (settings.saveFormat == ImageFormat::JPEG)
  {
    writeJpeg(stream, *imgToSave, settings);
  }
  else if (settings.saveFormat == ImageFormat::PNG)
  {
    writePng(stream, *imgToSave, settings);
  }
  else
  {
    throw std::runtime_error("Unsupported image data!");
  }
}

void ImageIO::SaveToBuffer(const Image& image, std::string& str, ImageSaveSettings settings)
{
  std::ostringstream outputStream;
  SaveToStream(image, outputStream, settings);
  str = outputStream.str();
}

void ImageIO::SaveToFile(std::filesystem::path imagePath, const Image& image, ImageSaveSettings settings)
{
  if (settings.saveFormat == ImageFormat::Auto)
  {
    settings.saveFormat = detectFormat(imagePath);
  }
  std::ofstream outputStream(imagePath, std::ios::binary | std::ios::trunc);
  SaveToStream(image, outputStream, settings);
  outputStream.close();
}
