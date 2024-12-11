#pragma once

#include <Arduino.h>

#include "SystemCommandType.h"

namespace Command {
namespace System {
struct ClearStatusFlagData {
  uint32_t mask;
};

struct __attribute__((packed)) SetAssignedIdData {
  char id[24];
};

struct __attribute__((packed)) SystemCommand {
  SystemCommandType type;
  union {
    ClearStatusFlagData clearStatusFlag;
    SetAssignedIdData assignedId;
  };
};
}  // namespace System
}  // namespace Command