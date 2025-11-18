#pragma once

#include <Arduino.h>
#include <FreeRTOS.h>
#include <SPI.h>
#include <Wire.h>
#include <queue.h>

#include <functional>
#include <optional>

#include "Eventing/EventingService.h"
#include "IModule.h"
#include "Networking/NetworkingService.h"

namespace Modules {

class BaseModuleImpl : public IModule {
 public:
  explicit BaseModuleImpl(const Configuration::Module& config);
  virtual ~BaseModuleImpl();

  uint16_t getId() const override { return _id; }
  const Configuration::Module& config() const override { return _config; }

 protected:
  ModuleError_t init() override;
  virtual ModuleError_t initModule() = 0;

  const Configuration::Module& getConfig() const { return _config; }

  TwoWire& WireInstance;
  arduino::HardwareSPI& SpiInstance;
  SerialUART& UartInstance;

 private:
  const Configuration::Module& _config;
  uint16_t _id;
  EventingService* _eventService;

  std::function<void(void)> _onInterruptCb;
  int8_t _registeredIntPin;
};

}  // namespace Modules