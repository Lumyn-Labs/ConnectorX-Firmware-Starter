#pragma once

#include <Arduino.h>
#include <FreeRTOS.h>
#include <stream_buffer.h>

#include <memory>
#include <string_view>

#include "ConfigurationParser/ConfigurationValidator.h"
#include "Eventing/EventingService.h"
#include "INetworkingAdapter.h"
#include "lumyn/util/serial/COBSEncoder.h"
#include "lumyn/util/serial/LumynTP.h"
#include "lumyn/util/serial/PacketSerial.h"

namespace Networking {
class BaseStreamAdapter : public INetworkingAdapter {
 public:
  BaseStreamAdapter(Stream& stream, std::string_view txTaskName,
                    std::string_view rxTaskName);

  void init(uint16_t transmissionQueueSize) override;
  void start(uint32_t stackSize, UBaseType_t priority) override;
  void writeTransmission(Transmission::Transmission* transmission) override;
  void onTransmission(
      std::function<void(Transmission::Transmission*)> cb) override;

  const char* name(void) const override { return _txTaskName.data(); }

  uint32_t maxMTU(void) const override {
    return lumyn::internal::Packet::Traits::MAX_PACKET_BODY_SIZE * 65536;
  }

 protected:
  virtual void initStream(void) = 0;
  virtual bool isConnected(void) = 0;
  virtual void transmitTask(void) = 0;
  virtual void receiveTask(void) = 0;

  QueueHandle_t _txQueue;  // Holds Transmission*
  std::unique_ptr<lumyn::internal::StandardPacketSerial> _serial;
  std::unique_ptr<lumyn::internal::StandardLumynTP> _lumynTp;
  lumyn::internal::COBSEncoder _encoder;

 private:
  static void transmitTaskImpl(void* _this);
  static void receiveTaskImpl(void* _this);

  EventingService* _evtService = &EventService;
  Stream& _stream;
  uint16_t _txQueueSize;

  TaskHandle_t _transmitTask;
  TaskHandle_t _receiveTask;
  std::string_view _txTaskName;
  std::string_view _rxTaskName;

  std::function<void(Transmission::Transmission*)> _onTransmissionCb;
};
}  // namespace Networking