#pragma once

#include <Arduino.h>
#include <CANBus-SOLDERED.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>

#include <functional>
#include <optional>
#include <vector>

#include "Networking/adapters/INetworkingAdapter.h"
#include "lumyn/domain/transmission/Transmission.h"
#include "lumyn/networking/FRCDefinitions.h"
#include "lumyn/util/serial/COBSEncoder.h"
#include "lumyn/util/serial/LumynTP.h"
#include "lumyn/util/serial/PacketSerial.h"

#ifndef IRAM_ATTR
#define IRAM_ATTR
#endif

namespace Transmission = lumyn::internal::Transmission;

namespace Networking {

class CAN20Adapter : public INetworkingAdapter {
 public:
  CAN20Adapter(byte csPin, byte intPin,
               MCP_CLOCK_T clockSpeed = MCP2518FD_40MHz,
               MCP_BITTIME_SETUP bitrate = CAN_1000KBPS);
  ~CAN20Adapter();

  void init(uint16_t transmissionQueueSize) override;
  void start(uint32_t stackSize, UBaseType_t priority) override;
  void writeTransmission(Transmission::Transmission* transmission) override;
  void onTransmission(
      std::function<void(Transmission::Transmission*)> callback) override;
  const char* name(void) const override { return "CAN20"; }
  uint32_t maxMTU(void) const override {
    return lumyn::internal::CANPacket::Traits::MAX_PACKET_BODY_SIZE * 256;
  }

  // CAN-specific configuration: Device and RIO listeners
  void configureFilters();

 private:
  static void canISRStatic();
  static CAN20Adapter* _instance;

  void handleInterrupt();  // Just sets notification, no I/O
  static void transmitTaskImpl(void* _this);
  static void receiveTaskImpl(void* _this);
  void transmitTask();
  void receiveTask();

  struct CANMessage {
    lumyn::internal::frc::CanId id;
    uint8_t data[8];
    uint8_t len;
    bool ext;
    bool rtr;
  };

  std::optional<lumyn::internal::CANPacket> handleCANMessage(
      const CANMessage& msg);

  CANBus _can;
  byte _intPin;
  MCP_CLOCK_T _clockSpeed;
  MCP_BITTIME_SETUP _bitrate;

  QueueHandle_t _txQueue;  // Holds Transmission*
  std::unique_ptr<lumyn::internal::CANPacketSerial> _serial;
  std::unique_ptr<lumyn::internal::CANLumynTP> _lumynTp;
  lumyn::internal::COBSEncoder _encoder;

  TaskHandle_t _transmitTask;
  TaskHandle_t _receiveTask;
  std::string_view _txTaskName;
  std::string_view _rxTaskName;

  std::function<void(Transmission::Transmission*)> _onTransmissionCb;

  volatile bool _initialized;
  uint16_t _deviceId;
};

}  // namespace Networking