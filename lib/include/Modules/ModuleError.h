#pragma once

#include <Arduino.h>

namespace Modules {
enum class ModuleErrorType_t : uint8_t {
  None,
  InvalidConfiguration,
  InitError,
  PinNotAvailable,
  MissingDeviceType,
  Timeout,
};

struct ModuleError_t {
  ModuleErrorType_t errorType;
  uint8_t additionalInformation[4];
};
}  // namespace Modules