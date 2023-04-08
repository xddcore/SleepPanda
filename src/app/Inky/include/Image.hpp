#pragma once

#include "Color.hpp"
#include "BoundingBox.hpp"
#include "base_resample.h"

#include <vector>
#include <string>
#include <memory>

// All of the below enums and settings structs should be inside
// the Image class but cannot due to a gcc and clang bug

enum class PixelFormat : int
{
  RGBA = 0,
  IndexedColor = 1
};

enum class ScaleMode
{
  Stretch,
  Fit,
  Fill
};

enum class InterpolationMode
{
  Auto = -1,
  Nearest  = (int) base::KernelTypeNearest,
  Average  = (int) base::KernelTypeAverage,
  Bilinear = (int) base::KernelTypeBilinear,
  Bicubic  = (int) base::KernelTypeBicubic,
  Mitchell = (int) base::KernelTypeMitchell,
  Cardinal = (int) base::KernelTypeCardinal,
  BSpline  = (int) base::KernelTypeBSpline,
  Lanczos  = (int) base::KernelTypeLanczos,
  Lanczos2 = (int) base::KernelTypeLanczos2,
  Lanczos3 = (int) base::KernelTypeLanczos3,
  Lanczos4 = (int) base::KernelTypeLanczos4,
  Lanczos5 = (int) base::KernelTypeLanczos5,
  Catmull  = (int) base::KernelTypeCatmull,
  Gaussian = (int) base::KernelTypeGaussian,
};

enum class DitherMode
{
  Diffusion, // Uses Floyd–Steinberg dithering algo
  Pattern // Uses classic 17 pattern swatches
};

struct DitherSettings
{
  // When converting from RGBA to an Inky image mode, this dithering mode will be used.
  DitherMode ditherMode = DitherMode::Diffusion;

  // Lower accuracy allows a cleaner result
  // Sane values: (0.5 - 1.0)
  float ditherAccuracy = 1.0f;
};

// Operations that flip and rotate the image
// NOTE: these correspond 1:1 with the exif orientation values
// but the names describe the forward transform rather than the inverse
enum class FlipRotateOperation : uint8_t
{
  None = 1,
  Mirror = 2,
  Rotate180 = 3,
  Rotate180Mirror = 4,
  Rotate90Mirror = 5, 
  Rotate90 = 6, 
  Rotate270Mirror = 7,
  Rotate270 = 8
};

struct ScaleSettings
{
  // Determines aspect and scale of image resizing
  // "Stretch" changes the source image aspect ratio to match the destination image
  // "Fill" maintains the source image aspect ratio and scales it large enough to fill all destination pixels
  // "Fit" maintains the source image aspect ratio and scales it to fit inside the destination image
  ScaleMode scaleMode = ScaleMode::Stretch;

  InterpolationMode interpolationMode = InterpolationMode::Auto;

  // When scaling or cropping, this determines what color fills in the background if not all destination pixels are covered
  RGBAColor backgroundColor {255, 255, 255, 255};
};

class Image
{
public:
    // Construct an RGBA image with no data
    Image();

    // Construct an RGBA image with the specified size, allocating memory.
    Image(int width, int height);

    // Construct an indexed image with the specified size and color map, allocating memory.
    Image(int width, int height, IndexedColorMap colorMap);

    // Get a pointer to the first pixel element
    uint8_t* data();

    // Get a const pointer to the first pixel element
    const uint8_t* data() const;

    // Get the image width
    int width() const;

    // Get the image height
    int height() const;

    // Get the image format
    PixelFormat format() const;

    const IndexedColorMap& colorMap() const;

    // Get the bounding box of this image's pixel grid
    BoundingBox bounds() const;

    // Get the number of bytes per pixel
    int bytesPerPixel() const;

    // Convert an image to indexed format. Specify a destination image for the conversion
    // or leave dest as nullptr to perform an in-place conversion.
    void toIndexed(IndexedColorMap colorMap, DitherSettings settings = DitherSettings());
    void toIndexed(Image& dest, IndexedColorMap colorMap, DitherSettings settings = DitherSettings()) const;

    // Convert an image to RGBA format
    void toRGBA();
    void toRGBA(Image& dest) const;

    // Rotate and/or flip the image
    void rotateFlip(FlipRotateOperation op);
    void rotateFlip(Image& dest, FlipRotateOperation op) const;

    // Scale the image to the specified size. Specify a destination image for the operation
    // or leave dest as nullptr to perform the operation in-place.
    void scale(int width, int height, ScaleSettings settings = {});
    void scale(Image& dest, int width, int height, ScaleSettings settings = {}) const;

    // Crop the image to the specified size rectangle. 
    // "Out of bounds" x, y, width, and height are ok, and will infill with a background color.
    // Specify a destination image for the operation or leave dest as nullptr to perform the operation in-place.
    void crop(int x, int y, int width, int height, ScaleSettings settings = {});
    void crop(Image& dest, int x, int y, int width, int height, ScaleSettings settings = {}) const;
private:
    friend class ImageIO;
    int width_, height_;
    PixelFormat format_;
    std::vector<uint8_t> data_;
    IndexedColorMap colorMap_;
};
