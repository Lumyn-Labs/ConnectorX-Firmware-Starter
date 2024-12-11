#pragma once

#include "ConfigurationParser/configs/Bitmap.h"

namespace Bitmap {
struct BitmapInstance {
  Configuration::Bitmap config;
  uint16_t frameCount;
};
}  // namespace Bitmap