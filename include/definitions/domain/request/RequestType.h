#pragma once

#include <Arduino.h>

namespace Request {
enum class RequestType : uint8_t {
  Handshake = 0,
  Status,
  ProductSKU,
  ProductSerialNumber,
  ConfigHash,
  AssignedId,
  Faults,
  DeviceStatus,
  DeviceData,
  LEDChannelStatus,
  LEDZoneStatus,
  LatestEvent,
  EventFlags,
  ModuleList,
};

enum class HostConnectionSource : uint8_t {
  Unknown = 0,
  Studio,
  Roborio,
};
};  // namespace Request