#pragma once

#include <Arduino.h>

#include <optional>
#include <string_view>
#include <vector>

#include "ConfigurationParser/configs/Zone.h"

namespace Configuration {
struct Channel {
  std::string key;
  std::string id;
  uint16_t length;
  std::optional<uint8_t> brightness;
  std::vector<Zone> zones;
};
}  // namespace Configuration