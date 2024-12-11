#pragma once

#include <Arduino.h>

#include "led/LEDCommand.h"
#include "system/SystemCommand.h"

namespace Command {
// 8-bit identifier for API grouping
enum class APIGroupType : uint8_t {
  System,
  LED,
  Device,
};
}  // namespace Command