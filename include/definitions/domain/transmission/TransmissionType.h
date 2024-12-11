#pragma once

#include <Arduino.h>

namespace Transmission {
enum class TransmissionType : uint8_t {
  Request = 0,
  Response,
  Event,
  Command,
  File,
  ModuleData
};
}  // namespace Transmission