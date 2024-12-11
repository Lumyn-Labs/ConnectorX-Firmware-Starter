#pragma once

#include <Arduino.h>
#include <FreeRTOS.h>
#include <stream_buffer.h>

#include <string_view>

#include "ConfigurationParser/ConfigurationValidator.h"
#include "Eventing/EventingService.h"
#include "INetworkingAdapter.h"

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

 protected:
  virtual void initStream(void) = 0;
  virtual bool isConnected(void) = 0;
  virtual void transmitTask(void) = 0;
  virtual void receiveTask(void) = 0;

  QueueHandle_t _txQueue;

 private:
  static void transmitTaskImpl(void* _this);
  static void receiveTaskImpl(void* _this);

  EventingService* _evtService = &EventService;
  Stream& _stream;
  uint16_t _txQueueSize;

  StreamBufferHandle_t _streamBuf;
  TaskHandle_t _transmitTask;
  TaskHandle_t _receiveTask;
  std::string_view _txTaskName;
  std::string_view _rxTaskName;

  std::function<void(Transmission::Transmission*)> _onTransmissionCb;
};
}  // namespace Networking