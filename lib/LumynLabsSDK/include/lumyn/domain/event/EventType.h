#pragma once

#include <cstdint>

#include "lumyn/types/events.h"
#include "lumyn/domain_types.h"
#include "lumyn/internal_names.h"

namespace lumyn::event
{
  constexpr uint8_t kEventBitCount = sizeof(uint32_t) * 8;

  enum class EventType : uint32_t
  {
    BeginInitialization = LUMYN_EVENT_BEGIN_INITIALIZATION,
    FinishInitialization = LUMYN_EVENT_FINISH_INITIALIZATION,
    Enabled = LUMYN_EVENT_ENABLED,
    Disabled = LUMYN_EVENT_DISABLED,
    Connected = LUMYN_EVENT_CONNECTED,
    Disconnected = LUMYN_EVENT_DISCONNECTED,
    Error = LUMYN_EVENT_ERROR,
    FatalError = LUMYN_EVENT_FATAL_ERROR,
    RegisteredEntity = LUMYN_EVENT_REGISTERED_ENTITY,
    Custom = LUMYN_EVENT_CUSTOM,
    PinInterrupt = LUMYN_EVENT_PIN_INTERRUPT,
    HeartBeat = LUMYN_EVENT_HEARTBEAT,
    OTA = LUMYN_EVENT_OTA,
    Module = LUMYN_EVENT_MODULE,
  };

  enum class Status : int8_t
  {
    Unknown = LUMYN_STATUS_UNKNOWN,
    Booting = LUMYN_STATUS_BOOTING,
    Active = LUMYN_STATUS_ACTIVE,
    Error = LUMYN_STATUS_ERROR,
    Fatal = LUMYN_STATUS_FATAL,
  };
} // namespace lumyn::event

namespace lumyn::internal::Eventing
{
  LUMYN_INTERNAL_NAME(EventType, lumyn::event::EventType, "lumyn::internal::Eventing::EventType")
  LUMYN_INTERNAL_NAME(Status, lumyn::event::Status, "lumyn::internal::Eventing::Status")
} // namespace lumyn::internal::Eventing

#ifdef LUMYN_INTERNAL_NAMES
namespace lumyn::internal::Eventing
{
  using lumyn::event::kEventBitCount;
}
#endif
