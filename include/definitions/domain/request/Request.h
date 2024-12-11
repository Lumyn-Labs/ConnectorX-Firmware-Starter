#pragma once

#include <Arduino.h>

#include "RequestType.h"

namespace Request {
struct __attribute__((packed)) RequestHandshakeInfo {
  HostConnectionSource hostSource;
};
struct __attribute__((packed)) RequestStatusInfo {};
struct __attribute__((packed)) RequestProductSKUInfo {};
struct __attribute__((packed)) RequestProductSerialNumberInfo {};
struct __attribute__((packed)) RequestConfigHashInfo {};
struct __attribute__((packed)) RequestAssignedIdInfo {};
struct __attribute__((packed)) RequestFaultsInfo {};
struct __attribute__((packed)) RequestDeviceStatusInfo {
  uint16_t deviceId;
};
struct __attribute__((packed)) RequestDeviceDataInfo {
  uint16_t deviceId;
};
struct __attribute__((packed)) RequestLEDChannelStatusInfo {
  uint16_t channelId;
};
struct __attribute__((packed)) RequestLEDZoneStatusInfo {
  uint16_t zoneId;
};
struct __attribute__((packed)) RequestLatestEventInfo {};
struct __attribute__((packed)) RequestEventFlagsInfo {};

struct __attribute__((packed)) Request {
  RequestType type;
  // Randomized request ID to correlate with the Response
  uint32_t id;
  union {
    RequestHandshakeInfo handshake;
    RequestStatusInfo status;
    RequestProductSKUInfo productSku;
    RequestProductSerialNumberInfo productSerialNumber;
    RequestConfigHashInfo configHash;
    RequestAssignedIdInfo assignedId;
    RequestFaultsInfo faults;
    RequestDeviceStatusInfo deviceStatus;
    RequestDeviceDataInfo deviceData;
    RequestLEDChannelStatusInfo ledChannelStatus;
    RequestLEDZoneStatusInfo ledZoneStatus;
    RequestLatestEventInfo latestEvent;
    RequestEventFlagsInfo eventFlags;
  };
};
}  // namespace Request