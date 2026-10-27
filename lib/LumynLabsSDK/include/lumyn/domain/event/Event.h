#pragma once

#include <cstring>
#include <cstdlib>
#include <new>
#include <utility>

#include "lumyn/domain/event/EventType.h"
#include "lumyn/packed.h"
#include "lumyn/util/RefCounted.h"
#include "lumyn/connectorx_types.h"
#include "lumyn/types/events.h"

namespace lumyn::event {

PACK(struct BeginInitInfo{});
PACK(struct FinishInitInfo{});
PACK(struct EnabledInfo{});

enum class DisabledCause : uint8_t {
  NoHeartbeat = LUMYN_DISABLED_CAUSE_NO_HEARTBEAT,
  Manual = LUMYN_DISABLED_CAUSE_MANUAL,
  EStop = LUMYN_DISABLED_CAUSE_ESTOP,
  Restart = LUMYN_DISABLED_CAUSE_RESTART,
};

PACK(struct DisabledInfo { DisabledCause cause; });

enum class ConnectionType : uint8_t {
  USB = LUMYN_CONNECTION_TYPE_USB,
  WebUSB = LUMYN_CONNECTION_TYPE_WEBUSB,
  I2C = LUMYN_CONNECTION_TYPE_I2C,
  CAN = LUMYN_CONNECTION_TYPE_CAN,
  UART = LUMYN_CONNECTION_TYPE_UART,
};

PACK(struct ConnectedInfo { ConnectionType type; });
PACK(struct DisconnectedInfo { ConnectionType type; });

enum class ErrorType : uint8_t {
  FileNotFound = CONNECTORX_ERROR_TYPE_FILE_NOT_FOUND,
  InvalidFile = CONNECTORX_ERROR_TYPE_INVALID_FILE,
  EntityNotFound = CONNECTORX_ERROR_TYPE_ENTITY_NOT_FOUND,
  DeviceMalfunction = CONNECTORX_ERROR_TYPE_DEVICE_MALFUNCTION,
  QueueFull = CONNECTORX_ERROR_TYPE_QUEUE_FULL,
  LedStrip = CONNECTORX_ERROR_TYPE_LED_STRIP,
  LedMatrix = CONNECTORX_ERROR_TYPE_LED_MATRIX,
  InvalidAnimationSequence = CONNECTORX_ERROR_TYPE_INVALID_ANIMATION_SEQUENCE,
  InvalidChannel = CONNECTORX_ERROR_TYPE_INVALID_CHANNEL,
  DuplicateID = CONNECTORX_ERROR_TYPE_DUPLICATE_ID,
  InvalidConfigUpload = CONNECTORX_ERROR_TYPE_INVALID_CONFIG_UPLOAD,
  ModuleError = CONNECTORX_ERROR_TYPE_MODULE_ERROR,
};

// ConnectorX's error types name the core's application slots; the rest are
// the core's own and must keep its values.
static_assert(int{CONNECTORX_ERROR_TYPE_FILE_NOT_FOUND} == LUMYN_ERROR_TYPE_FILE_NOT_FOUND &&
              int{CONNECTORX_ERROR_TYPE_INVALID_FILE} == LUMYN_ERROR_TYPE_INVALID_FILE &&
              int{CONNECTORX_ERROR_TYPE_ENTITY_NOT_FOUND} == LUMYN_ERROR_TYPE_ENTITY_NOT_FOUND &&
              int{CONNECTORX_ERROR_TYPE_DEVICE_MALFUNCTION} == LUMYN_ERROR_TYPE_DEVICE_MALFUNCTION &&
              int{CONNECTORX_ERROR_TYPE_QUEUE_FULL} == LUMYN_ERROR_TYPE_QUEUE_FULL &&
              int{CONNECTORX_ERROR_TYPE_DUPLICATE_ID} == LUMYN_ERROR_TYPE_DUPLICATE_ID &&
              int{CONNECTORX_FATAL_ERROR_TYPE_INIT_ERROR} == LUMYN_FATAL_ERROR_TYPE_INIT_ERROR &&
              int{CONNECTORX_FATAL_ERROR_TYPE_START_TASK} == LUMYN_FATAL_ERROR_TYPE_START_TASK &&
              int{CONNECTORX_FATAL_ERROR_TYPE_CREATE_QUEUE} == LUMYN_FATAL_ERROR_TYPE_CREATE_QUEUE,
              "connectorx.domain.toml's error types disagree with the core's");

PACK(struct ErrorInfo {
  ErrorType type;
  char message[16];
});

enum class FatalErrorType : uint8_t {
  InitError = CONNECTORX_FATAL_ERROR_TYPE_INIT_ERROR,
  BadConfig = CONNECTORX_FATAL_ERROR_TYPE_BAD_CONFIG,
  StartTask = CONNECTORX_FATAL_ERROR_TYPE_START_TASK,
  CreateQueue = CONNECTORX_FATAL_ERROR_TYPE_CREATE_QUEUE,
};

PACK(struct FatalErrorInfo {
  FatalErrorType type;
  char message[16];
});

PACK(struct ErrorFlags {
  union {
    struct {
  uint16_t nonFatalErrors;
  uint16_t fatalErrors;
    };
    uint32_t errors;
  };

  void raiseError(ErrorType error) {
    nonFatalErrors |= (1 << static_cast<uint32_t>(error));
  }
  void raiseError(FatalErrorType error) {
    fatalErrors |= (1 << static_cast<uint32_t>(error));
  }
  void clearError(ErrorType error) {
    nonFatalErrors &= ~(1 << static_cast<uint16_t>(error));
  }
  void clearError(FatalErrorType error) {
    fatalErrors &= ~(1 << static_cast<uint16_t>(error));
  }
  void clearError(uint32_t bitmask) { errors &= ~bitmask; }
  bool isErrorSet(ErrorType error) const {
    return nonFatalErrors & (1 << static_cast<uint16_t>(error));
  }
  bool isErrorSet(FatalErrorType error) const {
    return fatalErrors & (1 << static_cast<uint16_t>(error));
  }
});

PACK(struct RegisteredEntityInfo { uint16_t id; });

PACK(struct CustomInfo {
  uint8_t type;
  uint8_t data[16];
  uint8_t length;
});

PACK(struct PinInterruptInfo {
  uint8_t pin;
  void* param;
});

PACK(struct HeartBeatInfo {
  Status status;
  uint8_t enabled;
  uint8_t connectedUSB;
  uint8_t canOK;
});

class ExtraMessagePayload : public internal::RefCounted {
 public:
  uint16_t length;
  uint8_t data[];  // Flexible array member

  static ExtraMessagePayload* create(const uint8_t* msgData,
                                     uint16_t msgLength) {
    if (!msgData || msgLength == 0) return nullptr;

    // Allocate payload + data in one block
    void* mem = malloc(sizeof(ExtraMessagePayload) + msgLength);
    if (!mem) return nullptr;

    auto* payload = new (mem) ExtraMessagePayload();
    payload->length = msgLength;
    memcpy(payload->data, msgData, msgLength);

    return payload;
  }

  static ExtraMessagePayload* create(const char* str) {
    if (!str) return nullptr;
    size_t len = strlen(str) + 1;  // +1 for null terminator
    return create(reinterpret_cast<const uint8_t*>(str),
                  static_cast<uint16_t>(len));
  }

  /**
   * @brief Override unref to properly deallocate malloc'd memory.
   * 
   * Since this class is allocated with malloc() + placement new instead of operator new,
   * we must manually call the destructor and then use free() instead of delete.
   */
  void unref() const override
  {
    if (decrementRefCount())
    {
      // Explicitly call destructor for any cleanup
      const_cast<ExtraMessagePayload*>(this)->~ExtraMessagePayload();
      // Free using malloc's counterpart instead of delete
      free(const_cast<ExtraMessagePayload*>(this));
    }
  }

 protected:
  ExtraMessagePayload() : RefCounted(), length(0) {}
  ~ExtraMessagePayload() override = default;
};

PACK(union EventData {
  BeginInitInfo beginInit;
  FinishInitInfo finishInit;
  EnabledInfo enabled;
  DisabledInfo disabled;
  ConnectedInfo connected;
  DisconnectedInfo disconnected;
  ErrorInfo error;
  FatalErrorInfo fatalError;
  RegisteredEntityInfo registeredEntity;
  CustomInfo custom;
  PinInterruptInfo pinInterrupt;
  HeartBeatInfo heartBeat;
});

PACK(struct EventHeader {
  EventType type;
  EventData data;
});

struct Event {
  EventHeader header;
  ExtraMessagePayload* extraMsg;

  Event()
      : header({.type = EventType::BeginInitialization}), extraMsg(nullptr) {
    memset(&header.data.beginInit, 0, sizeof(EventData));
  }

  Event(EventHeader hdr) : header(hdr), extraMsg(nullptr) {}

  Event(EventHeader hdr, const char* msg) : header(hdr), extraMsg(nullptr) {
    setExtraMessage(msg);
  }

  Event(const Event& other) : header(other.header), extraMsg(other.extraMsg) {
    if (extraMsg) {
      extraMsg->ref();
    }
  }

  Event(Event&& other) noexcept
      : header(other.header), extraMsg(other.extraMsg) {
    other.extraMsg = nullptr;
  }

  Event& operator=(const Event& other) {
    if (this != &other) {
      if (extraMsg) extraMsg->unref();

      header = other.header;
      extraMsg = other.extraMsg;

      if (extraMsg) extraMsg->ref();
    }
    return *this;
  }

  Event& operator=(Event&& other) noexcept {
    if (this != &other) {
      if (extraMsg) extraMsg->unref();

      header = std::move(other.header);
      extraMsg = other.extraMsg;

      other.extraMsg = nullptr;
    }
    return *this;
  }

  ~Event() {
    if (extraMsg) {
      extraMsg->unref();
    }
  }

  void setExtraMessage(const uint8_t* data, uint16_t length) {
    if (extraMsg) {
      extraMsg->unref();
      extraMsg = nullptr;
    }

    if (data && length > 0) {
      extraMsg = ExtraMessagePayload::create(data, length);
    }
  }

  void setExtraMessage(const char* str) {
    if (extraMsg) {
      extraMsg->unref();
      extraMsg = nullptr;
    }

    if (str) {
      extraMsg = ExtraMessagePayload::create(str);
    }
  }

  const uint8_t* getExtraMessage() const {
    return extraMsg ? extraMsg->data : nullptr;
  }

  uint16_t getExtraMessageLength() const {
    return extraMsg ? extraMsg->length : 0;
  }

  const char* getExtraMessageStr() const {
    return extraMsg ? reinterpret_cast<const char*>(extraMsg->data) : nullptr;
  }

  bool hasExtraMessage() const { return extraMsg != nullptr; }
};

static_assert(sizeof(Event) <= 32, "Event should be compact (~24-28 bytes)");

// A wire message's length: it fills all its bytes when it's that long, with
// no terminator.
template <size_t N>
inline size_t messageLength(const char (&message)[N]) {
  const void* end = memchr(message, '\0', N);
  return end ? static_cast<size_t>(static_cast<const char*>(end) - message) : N;
}

// The event as the C API shows it. Its extra_message is NULL: each caller
// copies the extra message the way its API owns it.
inline lumyn_event_t toCEvent(const Event& in) {
  lumyn_event_t out;
  memset(&out, 0, sizeof(out));
  out.type = static_cast<lumyn_event_type_t>(in.header.type);
  const EventData& data = in.header.data;
  // The C enums are wider than the packed wire enums, so each field is
  // converted rather than the union copied.
  switch (in.header.type) {
    case EventType::Disabled:
      out.data.disabled.cause = static_cast<lumyn_disabled_cause_t>(data.disabled.cause);
      break;
    case EventType::Connected:
      out.data.connected.type = static_cast<lumyn_connection_type_t>(data.connected.type);
      break;
    case EventType::Disconnected:
      out.data.disconnected.type = static_cast<lumyn_connection_type_t>(data.disconnected.type);
      break;
    case EventType::Error:
      out.data.error.type = static_cast<lumyn_error_type_t>(data.error.type);
      memcpy(out.data.error.message, data.error.message, messageLength(data.error.message));
      break;
    case EventType::FatalError:
      out.data.fatal_error.type = static_cast<lumyn_fatal_error_type_t>(data.fatalError.type);
      memcpy(out.data.fatal_error.message, data.fatalError.message,
             messageLength(data.fatalError.message));
      break;
    case EventType::RegisteredEntity:
      out.data.registered_entity.id = data.registeredEntity.id;
      break;
    case EventType::Custom:
      out.data.custom.type = data.custom.type;
      out.data.custom.length = data.custom.length;
      memcpy(out.data.custom.data, data.custom.data, sizeof(out.data.custom.data));
      break;
    case EventType::PinInterrupt:
      out.data.pin_interrupt.pin = data.pinInterrupt.pin;
      break;
    case EventType::HeartBeat:
      out.data.heartbeat.status = static_cast<lumyn_status_t>(data.heartBeat.status);
      out.data.heartbeat.enabled = data.heartBeat.enabled;
      out.data.heartbeat.connected_usb = data.heartBeat.connectedUSB;
      out.data.heartbeat.can_ok = data.heartBeat.canOK;
      break;
    default:
      break;
  }
  return out;
}

}  // namespace lumyn::event

namespace lumyn::internal::Eventing
{
  LUMYN_INTERNAL_NAME(BeginInitInfo, lumyn::event::BeginInitInfo, "lumyn::internal::Eventing::BeginInitInfo")
  LUMYN_INTERNAL_NAME(FinishInitInfo, lumyn::event::FinishInitInfo, "lumyn::internal::Eventing::FinishInitInfo")
  LUMYN_INTERNAL_NAME(EnabledInfo, lumyn::event::EnabledInfo, "lumyn::internal::Eventing::EnabledInfo")
  LUMYN_INTERNAL_NAME(DisabledCause, lumyn::event::DisabledCause, "lumyn::internal::Eventing::DisabledCause")
  LUMYN_INTERNAL_NAME(DisabledInfo, lumyn::event::DisabledInfo, "lumyn::internal::Eventing::DisabledInfo")
  LUMYN_INTERNAL_NAME(ConnectionType, lumyn::event::ConnectionType, "lumyn::internal::Eventing::ConnectionType")
  LUMYN_INTERNAL_NAME(ConnectedInfo, lumyn::event::ConnectedInfo, "lumyn::internal::Eventing::ConnectedInfo")
  LUMYN_INTERNAL_NAME(DisconnectedInfo, lumyn::event::DisconnectedInfo, "lumyn::internal::Eventing::DisconnectedInfo")
  LUMYN_INTERNAL_NAME(ErrorType, lumyn::event::ErrorType, "lumyn::internal::Eventing::ErrorType")
  LUMYN_INTERNAL_NAME(ErrorInfo, lumyn::event::ErrorInfo, "lumyn::internal::Eventing::ErrorInfo")
  LUMYN_INTERNAL_NAME(FatalErrorType, lumyn::event::FatalErrorType, "lumyn::internal::Eventing::FatalErrorType")
  LUMYN_INTERNAL_NAME(FatalErrorInfo, lumyn::event::FatalErrorInfo, "lumyn::internal::Eventing::FatalErrorInfo")
  LUMYN_INTERNAL_NAME(ErrorFlags, lumyn::event::ErrorFlags, "lumyn::internal::Eventing::ErrorFlags")
  LUMYN_INTERNAL_NAME(RegisteredEntityInfo, lumyn::event::RegisteredEntityInfo, "lumyn::internal::Eventing::RegisteredEntityInfo")
  LUMYN_INTERNAL_NAME(CustomInfo, lumyn::event::CustomInfo, "lumyn::internal::Eventing::CustomInfo")
  LUMYN_INTERNAL_NAME(PinInterruptInfo, lumyn::event::PinInterruptInfo, "lumyn::internal::Eventing::PinInterruptInfo")
  LUMYN_INTERNAL_NAME(HeartBeatInfo, lumyn::event::HeartBeatInfo, "lumyn::internal::Eventing::HeartBeatInfo")
  LUMYN_INTERNAL_NAME(EventData, lumyn::event::EventData, "lumyn::internal::Eventing::EventData")
  LUMYN_INTERNAL_NAME(EventHeader, lumyn::event::EventHeader, "lumyn::internal::Eventing::EventHeader")
  LUMYN_INTERNAL_NAME(ExtraMessagePayload, lumyn::event::ExtraMessagePayload, "lumyn::internal::Eventing::ExtraMessagePayload")
  LUMYN_INTERNAL_NAME(Event, lumyn::event::Event, "lumyn::internal::Eventing::Event")
} // namespace lumyn::internal::Eventing
