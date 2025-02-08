#pragma once

#include <Arduino.h>

#include "Logger/Logger.h"
#include "Modules/BaseModule.h"
#include "Modules/PinGetter.h"

namespace Modules {
namespace Builtin {

class AnalogInputModule : public BaseModule {
 public:
  AnalogInputModule(const Configuration::Sensor& config) : BaseModule(config) {
    if (config.customConfig && config.customConfig->containsKey("pin")) {
      String pinName = (*config.customConfig)["pin"].as<String>();
      _pin = GetNamedPin(pinName.c_str());
    } else {
      SerialLogger.error("AnalogInputModule: Missing 'pin' in customConfig");
      _pin = 255;
    }

    if (config.customConfig) {
      if (config.customConfig->containsKey("scaleFactor") &&
          (*config.customConfig)["scaleFactor"]) {
        _scaleFactor = (*config.customConfig)["scaleFactor"].as<float>();
      }
      if (config.customConfig->containsKey("outputMin") &&
          (*config.customConfig)["outputMin"] &&
          config.customConfig->containsKey("outputMax") &&
          (*config.customConfig)["outputMax"]) {
        _outputMin = (*config.customConfig)["outputMin"].as<int32_t>();
        _outputMax = (*config.customConfig)["outputMax"].as<int32_t>();
        _enableMapping = true;
      }
    }
  }

  ModuleError_t initSensor(void) override {
    if (_pin == 255) {
      return {
          .errorType = ModuleErrorType_t::InitError,
          .additionalInformation = {_pin, 0, 0, 0},
      };
    }

    // invalid scale factor
    if (_scaleFactor <= 0) {
      SerialLogger.error("AnalogInputModule: Invalid scale factor %f",
                         _scaleFactor);
      return {
          .errorType = ModuleErrorType_t::InvalidConfiguration,
          .additionalInformation = {1, 0, 0, 0},
      };
    }

    // invalid mapping
    if (_enableMapping && _outputMin >= _outputMax) {
      SerialLogger.error(
          "AnalogInputModule: Invalid output min %d and output max %d",
          _outputMin, _outputMax);
      return {
          .errorType = ModuleErrorType_t::InvalidConfiguration,
          .additionalInformation = {2, 0, 0, 0},
      };
    }

    pinMode(_pin, INPUT);
    SerialLogger.verbose("AnalogInputModule: Initialized on analog pin %d",
                         _pin);

    return {
        .errorType = ModuleErrorType_t::None,
    };
  }

  std::optional<Eventing::Event> handleSubscribedEvent(
      const Eventing::Event& evt) override {
    SerialLogger.verbose("AnalogInputModule: Received event type %d", evt.type);
    return std::nullopt;  // No specific events to handle here
  }

  uint8_t poll(uint8_t* buf, ModuleError_t* err) override {
    AnalogInputPayload_t payload;

    int rawValue = analogRead(_pin);
    payload.rawValue = rawValue;

    if (_enableMapping) {
      payload.scaledValue =
          map(rawValue, _inputMin, _inputMax, _outputMin, _outputMax);
    } else {
      payload.scaledValue = static_cast<int32_t>(rawValue * _scaleFactor);
    }
    memcpy(buf, &payload, sizeof(payload));

    return sizeof(payload);
  }

 private:
  struct __attribute__((packed)) AnalogInputPayload_t {
    uint16_t rawValue;    // Raw analog value
    int32_t scaledValue;  // Scaled or mapped value
  };

  uint16_t _inputMin = 0;
  uint16_t _inputMax = 1023;

  uint8_t _pin;
  float _scaleFactor = 1.0f;  // Default scale factor
  bool _enableMapping = false;
  int32_t _outputMin = 0;
  int32_t _outputMax = 1023;
};

}  // namespace Builtin
}  // namespace Modules
