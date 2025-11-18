#pragma once

#include <Arduino.h>

#include "Logger/Logger.h"
#include "Modules/BaseModule.h"
#include "Modules/PinGetter.h"

namespace Modules {
namespace Builtin {

struct __attribute__((packed)) AnalogInputPayload_t {
  uint16_t rawValue;
  int32_t scaledValue;
};

class AnalogInputModule : public BaseModule<AnalogInputPayload_t> {
 public:
  AnalogInputModule(const Configuration::Module& config) : BaseModule(config) {
    parseConfig();
  }

 protected:
  ModuleError_t initModule() override {
    if (_pin == 255) {
      SerialLogger.error("AnalogInputModule: Invalid pin");
      return {
          .errorType = ModuleErrorType_t::InitError,
          .additionalInformation = {_pin, 0, 0, 0},
      };
    }

    if (_scaleFactor <= 0) {
      SerialLogger.error("AnalogInputModule: Invalid scale factor %f",
                         _scaleFactor);
      return {
          .errorType = ModuleErrorType_t::InvalidConfiguration,
          .additionalInformation = {1, 0, 0, 0},
      };
    }

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
    SerialLogger.info("AnalogInputModule: Initialized on pin %d", _pin);

    return {
        .errorType = ModuleErrorType_t::None,
        .additionalInformation = {0, 0, 0, 0},
    };
  }

  ModuleError_t readData(AnalogInputPayload_t* dataOut) override {
    if (!dataOut) {
      return {
          .errorType = ModuleErrorType_t::ReadError,
          .additionalInformation = {0xFF, 0, 0, 0},
      };
    }

    int rawValue = analogRead(_pin);
    dataOut->rawValue = rawValue;

    if (_enableMapping) {
      dataOut->scaledValue =
          map(rawValue, _inputMin, _inputMax, _outputMin, _outputMax);
    } else {
      dataOut->scaledValue = static_cast<int32_t>(rawValue * _scaleFactor);
    }

    SerialLogger.verbose("AnalogInputModule: Raw=%u, Scaled=%d",
                         dataOut->rawValue, dataOut->scaledValue);

    return {
        .errorType = ModuleErrorType_t::None,
        .additionalInformation = {0, 0, 0, 0},
    };
  }

 private:
  void parseConfig() {
    _pin = 255;
    _scaleFactor = 1.0f;
    _enableMapping = false;
    _inputMin = 0;
    _inputMax = 1023;
    _outputMin = 0;
    _outputMax = 1023;

    if (!config().customConfig) {
      SerialLogger.error("AnalogInputModule: Missing customConfig");
      return;
    }

    auto& cfg = *config().customConfig;

    if (cfg.containsKey("pin") && cfg["pin"]) {
      String pinName = cfg["pin"].as<String>();
      _pin = GetNamedPin(pinName.c_str());
      SerialLogger.verbose("AnalogInputModule: pin='%s' (%d)", pinName.c_str(),
                           _pin);
    }

    if (cfg.containsKey("scaleFactor") && cfg["scaleFactor"]) {
      _scaleFactor = cfg["scaleFactor"].as<float>();
      SerialLogger.verbose("AnalogInputModule: scaleFactor=%f", _scaleFactor);
    }

    if (cfg.containsKey("inputMin") && cfg["inputMin"]) {
      _inputMin = cfg["inputMin"].as<uint16_t>();
      SerialLogger.verbose("AnalogInputModule: inputMin=%u", _inputMin);
    }

    if (cfg.containsKey("inputMax") && cfg["inputMax"]) {
      _inputMax = cfg["inputMax"].as<uint16_t>();
      SerialLogger.verbose("AnalogInputModule: inputMax=%u", _inputMax);
    }

    if (cfg.containsKey("outputMin") && cfg["outputMin"] &&
        cfg.containsKey("outputMax") && cfg["outputMax"]) {
      _outputMin = cfg["outputMin"].as<int32_t>();
      _outputMax = cfg["outputMax"].as<int32_t>();
      _enableMapping = true;
      SerialLogger.verbose("AnalogInputModule: outputMin=%d, outputMax=%d",
                           _outputMin, _outputMax);
    }
  }

  uint8_t _pin;
  float _scaleFactor;
  bool _enableMapping;
  uint16_t _inputMin;
  uint16_t _inputMax;
  int32_t _outputMin;
  int32_t _outputMax;
};

}  // namespace Builtin
}  // namespace Modules