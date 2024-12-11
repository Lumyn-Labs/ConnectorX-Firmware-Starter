#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include <optional>

#include "ConfigurationParser/configs/Sensor.h"
#include "ModuleError.h"
#include "definitions/domain/event/Event.h"

namespace Modules {
class IModule {
 public:
  IModule(const Configuration::Sensor& config) : _config{config} {}

  virtual ModuleError_t init(void) = 0;
  virtual void loop(void) = 0;

  const Configuration::Sensor& config() const { return _config; }

 protected:
  const Configuration::Sensor& _config;
};
}  // namespace Modules