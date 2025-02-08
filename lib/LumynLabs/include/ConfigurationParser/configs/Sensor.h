#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include <optional>
#include <string>
#include <string_view>

#include "definitions/domain/module/ModuleInfo.h"

namespace Configuration {

struct Sensor {
  std::string id;
  std::string type;
  uint16_t pollingRateMs;
  SensorConnectionType connectionType;
  std::optional<JsonDocument> customConfig;
};
}  // namespace Configuration