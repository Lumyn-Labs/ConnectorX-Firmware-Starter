#pragma once

#include <Arduino.h>

namespace Configuration {
enum class NetworkType {
  I2C = 0,
  USB = 1,
};

struct I2CNetwork {
  uint8_t address;
};

struct Network {
  NetworkType type;
  union {
    I2CNetwork i2c;
  };
};
}  // namespace Configuration