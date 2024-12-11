#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include <optional>
#include <string>
#include <string_view>

namespace Configuration {
enum class SensorConnectionType { I2C = 0, SPI, UART, DIO, AIO };

struct Sensor {
  std::string id;
  std::string type;
  uint16_t pollingRateMs;
  SensorConnectionType connectionType;
  std::optional<JsonDocument> customConfig;
};
}  // namespace Configuration