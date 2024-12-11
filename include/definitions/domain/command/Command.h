#pragma once

#include <Arduino.h>

#include "CommandType.h"

namespace Command {
struct __attribute__((packed)) Command {
  APIGroupType type;
  union {
    LED::LEDCommand led;
    System::SystemCommand system;
  };
};
};  // namespace Command