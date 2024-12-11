#pragma once

#include <Arduino.h>

namespace Command {
namespace System {
enum class SystemCommandType : uint8_t {
  ClearStatusFlag = 0,
  SetAssignedId
};
}
}  // namespace Command