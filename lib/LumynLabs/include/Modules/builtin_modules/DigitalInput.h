#pragma once

#include <Arduino.h>

#include "Logger/Logger.h"
#include "Modules/BaseModule.h"
#include "Modules/PinGetter.h"

namespace Modules {
namespace Builtin {

struct __attribute__((packed)) DigitalInputPayload_t {
  uint8_t state;
};

class DigitalInputModule : public BaseModule<DigitalInputPayload_t> {
 public:
  DigitalInputModule(const Configuration::Module& config) : BaseModule(config) {
    parseConfig();
  }

 protected:
  ModuleError_t initModule() override {
    if (_pin == 255) {
      SerialLogger.error("DigitalInputModule: Invalid pin");
      return {
          .errorType = ModuleErrorType_t::InitError,
          .additionalInformation = {_pin, 0, 0, 0},
      };
    }

    // TODO: Allow for different pin modes
    pinMode(_pin, INPUT);

    return {
        .errorType = ModuleErrorType_t::None,
        .additionalInformation = {0, 0, 0, 0},
    };
  }

  ModuleError_t readData(DigitalInputPayload_t* dataOut) override {
    if (!dataOut) {
      return {
          .errorType = ModuleErrorType_t::ReadError,
          .additionalInformation = {0xFF, 0, 0, 0},
      };
    }

    if (_interruptEnabled) {
      if (_pinChanged) {
        dataOut->state = _lastPinState;
        _pinChanged = false;
        SerialLogger.verbose("DigitalInputModule: Pin %u changed to %d", _pin,
                             dataOut->state);
      } else {
        dataOut->state = 0;
      }
    } else {
      dataOut->state = digitalRead(_pin);
      SerialLogger.verbose("DigitalInputModule: Pin %u state is %u", _pin,
                           dataOut->state);
    }

    return {
        .errorType = ModuleErrorType_t::None,
        .additionalInformation = {0, 0, 0, 0},
    };
  }

 private:
  void parseConfig() {
    _pin = 255;
    _interruptEnabled = false;
    _interruptMode = PinStatus::CHANGE;
    _pinChanged = false;
    _lastPinState = 0;

    if (!config().customConfig) {
      SerialLogger.error("DigitalInputModule: Missing customConfig");
      return;
    }

    auto& cfg = *config().customConfig;

    if (cfg.containsKey("pin") && cfg["pin"]) {
      String pinName = cfg["pin"].as<String>();
      _pin = GetNamedPin(pinName.c_str());
      SerialLogger.verbose("DigitalInputModule: pin='%s' (%d)", pinName.c_str(),
                           _pin);
    }

    if (cfg.containsKey("interruptMode") && cfg["interruptMode"]) {
      String interruptModeStr = cfg["interruptMode"].as<String>();
      auto mode = parseInterruptMode(interruptModeStr);
      if (mode.has_value()) {
        _interruptMode = mode.value();
        _interruptEnabled = true;
        SerialLogger.verbose("DigitalInputModule: interruptMode='%s'",
                             interruptModeStr.c_str());
      } else {
        SerialLogger.error("DigitalInputModule: Invalid interruptMode '%s'",
                           interruptModeStr.c_str());
      }
    }
  }

  std::optional<PinStatus> parseInterruptMode(const String& str) {
    if (str == "LOW") return PinStatus::LOW;
    if (str == "HIGH") return PinStatus::HIGH;
    if (str == "CHANGE") return PinStatus::CHANGE;
    if (str == "FALLING") return PinStatus::FALLING;
    if (str == "RISING") return PinStatus::RISING;
    return std::nullopt;
  }

  uint8_t _pin;
  bool _interruptEnabled;
  PinStatus _interruptMode;
  volatile bool _pinChanged;
  volatile uint8_t _lastPinState;
};

}  // namespace Builtin
}  // namespace Modules