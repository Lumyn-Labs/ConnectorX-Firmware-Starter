#pragma once

#include <cinttypes>

namespace Files {
  enum class FileType : uint8_t {
    Transfer = 0,
    SendConfig,
    SetPixelBuffer
  };
}