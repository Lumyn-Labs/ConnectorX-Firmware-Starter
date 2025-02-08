#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <VL53L1X.h>

#include "Logger/Logger.h"
#include "Modules/BaseModule.h"

struct __attribute__((packed)) VL53L1XPayload_t {
  uint8_t valid;
  uint16_t dist;
};

namespace Modules {
namespace Builtin {

class VL53L1XModule : public BaseModule {
 public:
  VL53L1XModule(const Configuration::Sensor& config) : BaseModule(config) {
    _sensor.setBus(&WireInstance);
  }

  ModuleError_t initSensor(void) override {
    _sensor.setTimeout(500);
    if (!_sensor.init()) {
      SerialLogger.error("VL53L1XModule: Failed to initialize sensor");
      return {
          .errorType = ModuleErrorType_t::InitError,
          .additionalInformation = {1, 0, 0, 0},
      };
    }

    if (config().customConfig) {
      if (config().customConfig->containsKey("distanceMode") &&
          (*config().customConfig)["distanceMode"]) {
        String mode = (*config().customConfig)["distanceMode"].as<String>();
        if (mode == "Long") {
          _distanceMode = VL53L1X::Long;
        } else {
          _distanceMode = VL53L1X::Short;
        }
      }
      if (config().customConfig->containsKey("timingBudget") &&
          (*config().customConfig)["timingBudget"]) {
        _measurementTimingBudget =
            (*config().customConfig)["timingBudget"].as<uint32_t>();
      }
      if (config().customConfig->containsKey("continuousPeriod") &&
          (*config().customConfig)["continuousPeriod"]) {
        _continuousPeriod =
            (*config().customConfig)["continuousPeriod"].as<uint32_t>();
      } else {
        _continuousPeriod = _defaultContinuousPeriod;
      }
    } else {
      _distanceMode = VL53L1X::Long;
      _measurementTimingBudget = _defaultTimingBudget;
      _continuousPeriod = _defaultContinuousPeriod;
    }

    _sensor.setDistanceMode(_distanceMode);
    _sensor.setMeasurementTimingBudget(_measurementTimingBudget);

    SerialLogger.verbose(
        "VL53L1XModule: Sensor configured with distanceMode: %s, timingBudget: "
        "%u us, continuousPeriod: %lu ms",
        (_distanceMode == VL53L1X::Long ? "Long" : "Short"),
        _measurementTimingBudget, _continuousPeriod);

    _sensor.startContinuous(_continuousPeriod);

    return {
        .errorType = ModuleErrorType_t::None,
    };
  }

  std::optional<Eventing::Event> handleSubscribedEvent(
      const Eventing::Event& evt) override {
    SerialLogger.verbose("VL53L1XModule: Received event type %d", evt.type);
    return std::nullopt;
  }

  uint8_t poll(uint8_t* buf, ModuleError_t* err) override {
    VL53L1XPayload_t payload;
    uint16_t distance = _sensor.read(true);

    if (distance == _minValidDistance || distance >= _maxValidDistance) {
      payload.valid = 0;
      payload.dist = 0;
      SerialLogger.verbose(
          "VL53L1XModule: Invalid measurement detected (distance: %u)",
          distance);
    } else {
      payload.valid = 1;
      payload.dist = distance;
      SerialLogger.verbose("VL53L1XModule: Distance measurement: %u mm",
                           distance);
    }

    memcpy(buf, &payload, sizeof(payload));
    return sizeof(payload);
  }

 private:
  VL53L1X _sensor;
  uint32_t _continuousPeriod;  // Continuous measurement period in ms
  VL53L1X::DistanceMode _distanceMode = VL53L1X::Long;
  uint16_t _measurementTimingBudget = 50000;

  // Default values
  uint32_t _defaultContinuousPeriod = 50;
  uint16_t _defaultTimingBudget = 50000;
  uint16_t _minValidDistance = 0;
  uint16_t _maxValidDistance = 8190;
};

}  // namespace Builtin
}  // namespace Modules
