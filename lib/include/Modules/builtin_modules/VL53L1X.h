#pragma once

#include <Arduino.h>
#include <VL53L1X.h>

#include "Logger/Logger.h"
#include "Modules/BaseModule.h"

struct VL53L1XPayload_t {
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

  ModuleError_t initSensor(void) {
    _sensor.setTimeout(500);
    if (!_sensor.init()) {
      return {
          .errorType = ModuleErrorType_t::InitError,
          .additionalInformation = {1, 0, 0, 0},
      };
    }

    _sensor.setDistanceMode(VL53L1X::Long);
    _sensor.setMeasurementTimingBudget(50000);
    _sensor.startContinuous(50);

    SerialLogger.verbose("Done writing to VL53L1X");

    listenForEvent(Eventing::EventType::Enabled);

    return {
        .errorType = ModuleErrorType_t::None,
    };
  }

  std::optional<Eventing::Event> handleSubscribedEvent(
      const Eventing::Event& evt) override {
    // Handle your events here...
    SerialLogger.verbose("VL53L1X: got event: %d", evt.type);

    return std::nullopt;
  }

  uint8_t poll(uint8_t* buf, ModuleError_t* err) override {
    VL53L1XPayload_t payload;
    payload.dist = _sensor.read(true);
    payload.valid = 1;

    memcpy(buf, &payload, sizeof(payload));

    return sizeof(payload);
  }

 private:
  VL53L1X _sensor;
};
}  // namespace Builtin
}  // namespace Modules