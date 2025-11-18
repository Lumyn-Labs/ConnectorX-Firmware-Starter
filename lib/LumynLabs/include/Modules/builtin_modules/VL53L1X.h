#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <VL53L1X.h>

#include "Logger/Logger.h"
#include "Modules/BaseModule.h"

namespace Modules {
namespace Builtin {

struct __attribute__((packed)) VL53L1XPayload_t {
  uint8_t valid;
  uint16_t dist;
};

class VL53L1XModule : public BaseModule<VL53L1XPayload_t> {
 public:
  VL53L1XModule(const Configuration::Module& config) : BaseModule(config) {
    _sensor.setBus(&WireInstance);
  }

 protected:
  ModuleError_t initModule() override {
    _sensor.setTimeout(500);

    if (!_sensor.init()) {
      SerialLogger.error("VL53L1XModule: Failed to initialize sensor");
      return {
          .errorType = ModuleErrorType_t::InitError,
          .additionalInformation = {1, 0, 0, 0},
      };
    }

    parseConfig();

    _sensor.setDistanceMode(_distanceMode);
    _sensor.setMeasurementTimingBudget(_measurementTimingBudget);
    _sensor.startContinuous(_continuousPeriod);

    SerialLogger.info(
        "VL53L1XModule: Configured - Mode:%s, Timing:%uµs, Period:%lums",
        (_distanceMode == VL53L1X::Long ? "Long" : "Short"),
        _measurementTimingBudget, _continuousPeriod);

    return {
        .errorType = ModuleErrorType_t::None,
        .additionalInformation = {0, 0, 0, 0},
    };
  }

  ModuleError_t readData(VL53L1XPayload_t* dataOut) override {
    if (!dataOut) {
      return {
          .errorType = ModuleErrorType_t::ReadError,
          .additionalInformation = {0xFF, 0, 0, 0},
      };
    }

    uint16_t distance = _sensor.read(true);

    if (distance == _minValidDistance || distance >= _maxValidDistance) {
      dataOut->valid = 0;
      dataOut->dist = 0;
      SerialLogger.verbose("VL53L1XModule: Invalid measurement (distance=%u)",
                           distance);
    } else {
      dataOut->valid = 1;
      dataOut->dist = distance;
      SerialLogger.verbose("VL53L1XModule: Distance=%u mm", distance);
    }

    return {
        .errorType = ModuleErrorType_t::None,
        .additionalInformation = {0, 0, 0, 0},
    };
  }

 private:
  void parseConfig() {
    _distanceMode = VL53L1X::Long;
    _measurementTimingBudget = _defaultTimingBudget;
    _continuousPeriod = _defaultContinuousPeriod;

    if (!config().customConfig) {
      return;
    }

    auto& cfg = *config().customConfig;

    if (cfg.containsKey("distanceMode") && cfg["distanceMode"]) {
      String mode = cfg["distanceMode"].as<String>();
      _distanceMode = (mode == "Long") ? VL53L1X::Long : VL53L1X::Short;
      SerialLogger.verbose("VL53L1XModule: distanceMode='%s'", mode.c_str());
    }

    if (cfg.containsKey("timingBudget") && cfg["timingBudget"]) {
      _measurementTimingBudget = cfg["timingBudget"].as<uint32_t>();
      SerialLogger.verbose("VL53L1XModule: timingBudget=%u µs",
                           _measurementTimingBudget);
    }

    if (cfg.containsKey("continuousPeriod") && cfg["continuousPeriod"]) {
      _continuousPeriod = cfg["continuousPeriod"].as<uint32_t>();
      SerialLogger.verbose("VL53L1XModule: continuousPeriod=%lu ms",
                           _continuousPeriod);
    }
  }

  VL53L1X _sensor;

  VL53L1X::DistanceMode _distanceMode;
  uint32_t _measurementTimingBudget;
  uint32_t _continuousPeriod;

  static constexpr uint32_t _defaultTimingBudget = 50000;
  static constexpr uint32_t _defaultContinuousPeriod = 50;
  static constexpr uint16_t _minValidDistance = 0;
  static constexpr uint16_t _maxValidDistance = 8190;
};

}  // namespace Builtin
}  // namespace Modules