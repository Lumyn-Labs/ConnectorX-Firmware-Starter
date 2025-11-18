#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include <optional>
#include <string>
#include <string_view>

#include "lumyn/domain/module/ModuleInfo.h"

namespace Configuration {

struct Module {
  std::string id;
  std::string type;
  uint16_t pollingRateMs;
  lumyn::internal::ModuleInfo::ModuleConnectionType connectionType;
  std::optional<JsonDocument> customConfig;
};
}  // namespace Configuration