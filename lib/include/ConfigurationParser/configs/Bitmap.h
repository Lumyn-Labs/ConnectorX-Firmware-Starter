#pragma once

#include <Arduino.h>

#include <string_view>

namespace Configuration {
enum class BitmapType {
  Static = 0,
  Animated,
};

struct AnimatedBitmap {
  uint16_t frameDelay;
};

struct Bitmap {
  BitmapType type;
  std::string id;
  std::string path;
  union {
    AnimatedBitmap animatedBmp;
  };
};
}  // namespace Configuration