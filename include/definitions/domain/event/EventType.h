#pragma once

#include <Arduino.h>

namespace Eventing {
constexpr uint8_t kEventBitCount = sizeof(uint32_t) * 8;

enum class Status : int8_t { Unknown = -1, Booting = 0, Active, Error, Fatal };

enum class EventType : uint32_t {
  BeginInitialization = 0x00,
  FinishInitialization = 0x01 << 0,
  Enabled = 0x01 << 1,
  Disabled = 0x01 << 2,
  Connected = 0x01 << 3,
  Disconnected = 0x01 << 4,
  Error = 0x01 << 5,
  FatalError = 0x01 << 6,
  RegisteredEntity = 0x01 << 7,
  Custom = 0x01 << 8,
  PinInterrupt = 0x01 << 9,
  // AKA periodic status
  HeartBeat = 0x01 << 10,
};
}  // namespace Eventing