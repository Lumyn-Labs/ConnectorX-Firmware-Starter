#pragma once

#include <Arduino.h>

#include <string_view>

namespace Bitmap {
enum class BitmapType {
  Invalid = 0,
  Monochrome,
  Grayscale8Bit,
  RGB16Bit,
};

struct BitmapInfo {
  uint32_t width;
  uint32_t height;
  // Location of the actual pixel data
  uint32_t streamOffset;
  BitmapType type;
  uint16_t bitsPerPixel;
  // Number of bytes needed to store the pixel data
  uint32_t totalPixelDataSize;
};

struct FileHeader {
  uint16_t signature;
  uint32_t file_size;
  uint16_t reserved[2];
  uint32_t image_offset;
};

struct ImageHeader {
  uint32_t header_size;
  uint32_t image_width;
  uint32_t image_height;
  uint16_t color_planes;
  uint16_t bits_per_pixel;
  uint32_t compression_method;
  uint32_t image_size;
  uint32_t horizontal_resolution;
  uint32_t vertical_resolution;
  uint32_t colors_in_palette;
  uint32_t important_colors;
};
}  // namespace Bitmap