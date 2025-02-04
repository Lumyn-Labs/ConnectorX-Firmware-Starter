#pragma once

#include <Arduino.h>

#include "Logger/Logger.h"
#include "Modules/BaseModule.h"
#include "Modules/PinGetter.h"

namespace Modules {
namespace Builtin {

class DigitalInputModule : public BaseModule {
 public:
  DigitalInputModule(const Configuration::Sensor& config) : BaseModule(config) {
    if (config.customConfig && config.customConfig->containsKey("pin")) {
      String pinName = (*config.customConfig)["pin"].as<String>();
      _pin = GetNamedPin(pinName.c_str());
    } else {
      SerialLogger.error("DigitalInputModule: Missing 'pin' in customConfig");
      _pin = 255;
    }
  }

  ModuleError_t initSensor(void) override {
    if (config().customConfig &&
        config().customConfig->containsKey("interruptMode")) {
      String interruptModeStr =
          (*config().customConfig)["interruptMode"].as<String>();

      auto interruptMode = parseInterruptMode(interruptModeStr);
      if (!interruptMode) {
        return {.errorType = ModuleErrorType_t::InvalidConfiguration,
                .additionalInformation = {1, 0, 0, 0}};
      } else {
        _interruptMode = *interruptMode;
        _interruptEnabled = true;
        attachDigitalInterrupt(_pin, _interruptMode, [this]() {
          _pinChanged = true;
          _lastPinState = digitalReadFast(_pin);
        });
      }
    }

    if (_pin == 255) {
      return {
          .errorType = ModuleErrorType_t::InitError,
          .additionalInformation = {_pin, 0, 0, 0},
      };
    }

    pinMode(_pin, INPUT);
    SerialLogger.verbose("DigitalInputModule: Created with pin %u and mode %u",
                         _pin, _interruptMode);

    return {
        .errorType = ModuleErrorType_t::None,
    };
  }

  std::optional<Eventing::Event> handleSubscribedEvent(
      const Eventing::Event& evt) override {
    SerialLogger.verbose("DigitalInputModule: Received event type %lu",
                         evt.type);
    return std::nullopt;
  }

  uint8_t poll(uint8_t* buf, ModuleError_t* err) override {
    DigitalInputPayload_t payload;

    if (_interruptEnabled) {
      if (_pinChanged) {
        payload.state = _lastPinState;
        _pinChanged = false;
        SerialLogger.verbose("DigitalInputModule: Pin %u changed to %d", _pin,
                             payload.state);
      } else {
        payload.state = 0;
      }
    } else {
      payload.state = digitalRead(_pin);
      SerialLogger.verbose("DigitalInputModule: Pin %u state is %u", _pin,
                           payload.state);
    }

    memcpy(buf, &payload, sizeof(payload));

    return sizeof(payload);
  }

 private:
  struct __attribute__((packed)) DigitalInputPayload_t {
    uint8_t state;
  };

  volatile bool _pinChanged = false;
  volatile uint8_t _lastPinState = 0;
  bool _interruptEnabled = false;

  std::optional<PinStatus> parseInterruptMode(const String& str) {
    if (str == "LOW") {
      return PinStatus::LOW;
    } else if (str == "HIGH") {
      return PinStatus::HIGH;
    } else if (str == "CHANGE") {
      return PinStatus::CHANGE;
    } else if (str == "FALLING") {
      return PinStatus::FALLING;
    } else if (str == "RISING") {
      return PinStatus::RISING;
    } else {
      SerialLogger.error("DigitalInputModule: Unknown interruptMode '%s'",
                         str.c_str());
      return std::nullopt;
    }
  }

  uint8_t _pin;
  PinStatus _interruptMode;
};

}  // namespace Builtin
}  // namespace Modules
