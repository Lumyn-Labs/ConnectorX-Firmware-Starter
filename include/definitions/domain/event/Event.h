#pragma once

#include <Arduino.h>

#include "EventType.h"

namespace Eventing {
struct BeginInitInfo {};

struct FinishInitInfo {};

struct EnabledInfo {};

enum class DisabledCause : uint8_t {
  NoHeartbeat,
  Manual,
  EStop,
};

struct DisabledInfo {
  DisabledCause cause;
};

// TODO: Move somewhere more visible
enum class ConnectionType : uint8_t {
  USB,
  WebUSB,
  I2C,
  CAN,
};

struct ConnectedInfo {
  ConnectionType type;
};

struct DisconnectedInfo {
  ConnectionType type;
};

enum class ErrorType : uint8_t {
  FileNotFound = 0,
  InvalidFile,
  EntityNotFound,
  DeviceMalfunction,
  QueueFull,
  LedStrip,
  LedMatrix,
  InvalidAnimationSequence,
  InvalidChannel,
  DuplicateID,
  InvalidConfigUpload,
};

struct ErrorInfo {
  ErrorType type;
  char message[16];
};

enum class FatalErrorType : uint8_t {
  InitError = 0,
  BadConfig,
  StartTask,
  CreateQueue,
};

struct FatalErrorInfo {
  FatalErrorType type;
  char message[16];
};

struct ErrorFlags {
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
};

struct RegisteredEntityInfo {
  uint16_t id;
};

struct CustomInfo {
  uint8_t type;
  // TODO: Constant kMaxCustomDataLength?
  uint8_t data[16];
  uint8_t length;
};

struct PinInterruptInfo {
  uint8_t pin;
  void* param;
};

struct __attribute__((packed)) HeartBeatInfo {
  Status status;
  uint8_t enabled;
  uint8_t connectedUSB;
  uint8_t canOK;
};

struct Event {
  EventType type;

  union {
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
  };
};
}  // namespace Eventing