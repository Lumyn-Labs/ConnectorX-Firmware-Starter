#pragma once

#include <Arduino.h>
#include <FreeRTOS.h>
#include <SPI.h>
#include <Wire.h>
#include <queue.h>
#include <task.h>

#include <functional>

#include "Eventing/EventingService.h"
#include "IDCreator/IDCreator.h"
#include "Modules/IModule.h"
#include "Networking/NetworkingService.h"
#include "definitions/domain/transmission/Transmission.h"

namespace Modules {
class BaseModule : public IModule {
 public:
  BaseModule(const Configuration::Sensor& config);

  void loop(void) override;
  ModuleError_t init(void) override;

 protected:
  // TODO: Make pin an enum class?
  void attachDigitalInterrupt(uint8_t pin, PinStatus mode,
                              std::function<void(void)>);
  void listenForEvent(Eventing::EventType);
  /**
   * @brief Handle received events
   *
   * @return std::optional&lt;Eventing::Event&gt; If not std::nullopt, will send
   * the event
   */
  virtual std::optional<Eventing::Event> handleSubscribedEvent(
      const Eventing::Event&) = 0;
  virtual ModuleError_t initSensor(void) = 0;
  virtual uint8_t poll(uint8_t*, ModuleError_t*) = 0;

  TwoWire& WireInstance = Wire1;
  arduino::HardwareSPI& SpiInstance = SPI1;
  SerialUART& UartInstance = Serial2;

  static constexpr uint8_t kMaxPayloadLen = 16;

 private:
  // This callback gets passed to attachInterruptParams along with the event
  // queue as the param argument
  static void onInterruptImpl(void* eventQueueHandle);
  void handleAllEvents(const Eventing::Event&);
  void sendDeviceData(uint8_t len);

  uint16_t _id;
  NetworkingService* _networkingService = &NetworkService;
  EventingService* _eventService = &EventService;
  // TODO: Device status enum class + retrieval

  TickType_t _queueTimeout;
  // TODO: Max data size constant
  uint8_t _latestData[16];
  std::function<void(void)> _onInterruptCb;
  int8_t _registeredIntPin = -1;

  QueueHandle_t _eventQueue;
  uint16_t _eventQueueSize;
};
}  // namespace Modules