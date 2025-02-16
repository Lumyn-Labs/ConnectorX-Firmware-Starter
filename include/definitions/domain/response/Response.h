#pragma once

#include <Arduino.h>

#include "SystemService/Status.h"
#include "definitions/domain/event/Event.h"
#include "definitions/domain/module/ModuleInfo.h"
#include "definitions/domain/request/RequestType.h"

namespace Response {
struct __attribute__((packed)) ResponseStatusInfo {
  Eventing::Status status;
};
struct __attribute__((packed)) ResponseProductSKUInfo {
  uint16_t sku;
};
struct __attribute__((packed)) ResponseProductSerialNumberInfo {
  uint64_t serialNumber;
};
struct __attribute__((packed)) ResponseConfigHashInfo {
  uint8_t hash[16];
};
struct __attribute__((packed)) ResponseAssignedIdInfo {
  uint8_t valid;
  char id[24];
};
struct __attribute__((packed)) ResponseVersionInfo {
  uint8_t major;
  uint8_t minor;
  uint8_t patch;
};
struct __attribute__((packed)) ResponseHandshakeInfo {
  ResponseStatusInfo status;
  ResponseProductSKUInfo sku;
  ResponseProductSerialNumberInfo serNumber;
  ResponseConfigHashInfo configHash;
  ResponseAssignedIdInfo assignedId;
  ResponseVersionInfo version;

};
struct __attribute__((packed)) ResponseFaultsInfo {
  uint32_t faultFlags;
};
struct __attribute__((packed)) ResponseDeviceStatusInfo {
  uint16_t deviceId;
  uint8_t status;
};
struct __attribute__((packed)) ResponseDeviceDataInfo {
  uint16_t deviceId;
  // TODO: constant for max data size
  uint8_t data[16];
  uint8_t length;
};
struct __attribute__((packed)) ResponseLEDChannelStatusInfo {
  uint16_t channelId;
};
struct __attribute__((packed)) ResponseLEDZoneStatusInfo {
  uint16_t zoneId;
};
struct __attribute__((packed)) ResponseLatestEventInfo {
  Eventing::EventType eventType;
};
struct __attribute__((packed)) ResponseEventFlagsInfo {
  uint32_t eventFlags;
};

struct __attribute__((packed)) ResponseModuleListEntity {
  uint16_t moduleId;
  char name[24];
  char type[24];
  uint16_t pollingRateMs;
  SensorConnectionType connectionType;
};

struct __attribute__((packed)) ResponseModuleListInfo {
  uint16_t count;
  ResponseModuleListEntity modules[8];
};

struct __attribute__((packed)) Response {
  Request::RequestType type;
  // Should match the incoming Request's ID
  uint32_t id;
  union {
    ResponseHandshakeInfo handshake;
    ResponseStatusInfo status;
    ResponseProductSKUInfo productSku;
    ResponseProductSerialNumberInfo productSerialNumber;
    ResponseConfigHashInfo configHash;
    ResponseAssignedIdInfo assignedId;
    ResponseFaultsInfo faults;
    ResponseDeviceStatusInfo deviceStatus;
    ResponseDeviceDataInfo deviceData;
    ResponseLEDChannelStatusInfo ledChannelStatus;
    ResponseLEDZoneStatusInfo ledZoneStatus;
    ResponseLatestEventInfo latestEvent;
    ResponseEventFlagsInfo eventFlags;
    ResponseModuleListInfo moduleList;
  };
};

constexpr auto size = sizeof(Response);

}  // namespace Response