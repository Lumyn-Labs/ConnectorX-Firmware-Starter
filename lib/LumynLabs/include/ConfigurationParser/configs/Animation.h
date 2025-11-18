#pragma once

#include <Arduino.h>

#include <optional>
#include <string_view>

#include "lumyn/domain/command/led/LEDCommand.h"

namespace Configuration {
struct ActionColor {
  uint8_t r;
  uint8_t g;
  uint8_t b;

  static ActionColor fromCommand(lumyn::internal::Command::LED::AnimationColor color) {
    return {
        .r = color.r,
        .g = color.g,
        .b = color.b,
    };
  }
};

struct Animation {
  std::string id;
  bool reversed;
  uint16_t delay;
  std::optional<ActionColor> color;
  std::optional<uint8_t> repeatCount;
};
}  // namespace Configuration