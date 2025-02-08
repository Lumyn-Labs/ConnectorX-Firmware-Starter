#pragma once

#include <stdint.h>

enum class SensorConnectionType : uint8_t { I2C = 0, SPI, UART, DIO, AIO };