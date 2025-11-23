#pragma once

#include <stdint.h>

#include <optional>
#include <vector>
#include <ArduinoJson.h>

#include "ConfigurationParser/configs/Module.h"
#include "Modules/ModuleError.h"
#include "lumyn/domain/event/Event.h"

namespace Modules {

class IModule {
 public:
  virtual ~IModule() = default;

  virtual ModuleError_t init() = 0;
  virtual ModuleError_t read(std::vector<uint8_t>& dataOut) = 0;
  virtual bool pushData(ArduinoJson::JsonVariantConst json) = 0;
  // TODO: Later
  // virtual std::optional<Eventing::Event> handleEvent(
  //     const Eventing::Event& evt) = 0;

  virtual uint16_t getId() const = 0;
  virtual const Configuration::Module& config() const = 0;
};

}  // namespace Modules