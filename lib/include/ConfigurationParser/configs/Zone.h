#pragma once

#include <Arduino.h>

#include <optional>
#include <string>

namespace Configuration {

enum class ZoneType {
  Strip = 0,
  Matrix,
};

struct ZoneStrip {
  uint16_t length;
  bool reversed;
};

struct ZoneMatrix {
  uint16_t rows;
  uint16_t cols;
  uint8_t orientation;
};

struct Zone {
  ZoneType type;
  std::string id;
  std::optional<uint8_t> brightness;
  union {
    ZoneStrip strip;
    ZoneMatrix matrix;
  };
};

}  // namespace Configuration