#pragma once

#include "ModuleDataType.h"

namespace ModuleData {
struct __attribute__((packed)) NewDataInfo {
  uint16_t id;
  uint8_t data[16];
  uint8_t len;
};

struct __attribute__((packed)) ModuleData {
  ModuleDataType type;
  union {
    NewDataInfo newData;
  };
};
}  // namespace ModuleData