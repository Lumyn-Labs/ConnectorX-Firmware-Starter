#pragma once

#include <Adafruit_AS7341.h>

#include "Logger/Logger.h"
#include "Modules/BaseModule.h"

namespace Modules {
namespace Builtin {

class AS7341Module : public BaseModule {
 public:
  AS7341Module(const Configuration::Sensor& config) : BaseModule(config) {}

  ModuleError_t initSensor(void) override {
    if (!_sensor.begin()) {
      SerialLogger.error("AS7341Module: Sensor initialization failed!");
      return {
          .errorType = ModuleErrorType_t::InitError,
          .additionalInformation = {1, 0, 0, 0},
      };
    }

    if (_config.customConfig) {
      if (_config.customConfig->containsKey("gain")) {
        _gain = (*_config.customConfig)["gain"].as<uint8_t>();
        _sensor.setGain(static_cast<as7341_gain_t>(_gain));
        SerialLogger.verbose("AS7341Module: Gain set to %d", _gain);
      }
      if (_config.customConfig->containsKey("atime")) {
        _atime = (*_config.customConfig)["atime"].as<uint8_t>();
        _sensor.setATIME(_atime);
        SerialLogger.verbose("AS7341Module: ATIME set to %d", _atime);
      }
      if (_config.customConfig->containsKey("astep")) {
        _astep = (*_config.customConfig)["astep"].as<uint16_t>();
        _sensor.setASTEP(_astep);
        SerialLogger.verbose("AS7341Module: ASTEP set to %d", _astep);
      }
      if (_config.customConfig->containsKey("rgbwConversion")) {
        _rgbwConversion = (*_config.customConfig)["rgbwConversion"].as<bool>();
        SerialLogger.verbose("AS7341Module: RGBW conversion is %s",
                             _rgbwConversion ? "enabled" : "disabled");
      }
    }

    return {
        .errorType = ModuleErrorType_t::None,
    };
  }

  std::optional<Eventing::Event> handleSubscribedEvent(
      const Eventing::Event& evt) override {
    SerialLogger.verbose("AS7341Module: Received event type %d", evt.type);
    // TODO: Handle events(?)
    return std::nullopt;
  }

  uint8_t poll(uint8_t* buf, ModuleError_t* err) override {
    if (!_sensor.readAllChannels()) {
      SerialLogger.error("AS7341Module: Error reading all channels!");
      *err = {
          .errorType = ModuleErrorType_t::Timeout,
          .additionalInformation = {2, 0, 0, 0},
      };
      return 0;
    }

    if (_rgbwConversion) {
      return pollRGBW(buf);
    } else {
      return pollRaw(buf);
    }
  }

 private:
  union AS7341Payload_t {
    struct {
      uint16_t channelF1;  // 415nm
      uint16_t channelF2;  // 445nm
      uint16_t channelF3;  // 480nm
      uint16_t channelF4;  // 515nm
      uint16_t channelF5;  // 555nm
      uint16_t channelF6;  // 590nm
      uint16_t channelF7;  // 630nm
      uint16_t channelF8;  // 680nm
    } rawData;
    struct {
      uint16_t red;
      uint16_t green;
      uint16_t blue;
      uint16_t white;
    } rgbwData;
  };

  /**
   * Poll the sensor and convert the results to RGBW.
   * Writes the payload into the provided buffer and returns the payload size.
   */
  uint8_t pollRGBW(uint8_t* buf) {
    AS7341Payload_t payload_rgbw;
    convertToRGBW(payload_rgbw);
    memcpy(buf, &payload_rgbw, sizeof(payload_rgbw));
    return sizeof(payload_rgbw);
  }

  /**
   * Poll the sensor and return the raw channel data.
   * Writes the payload into the provided buffer and returns the payload size.
   */
  uint8_t pollRaw(uint8_t* buf) {
    AS7341Payload_t payload_raw;
    payload_raw.rawData.channelF1 = _sensor.getChannel(AS7341_CHANNEL_415nm_F1);
    payload_raw.rawData.channelF2 = _sensor.getChannel(AS7341_CHANNEL_445nm_F2);
    payload_raw.rawData.channelF3 = _sensor.getChannel(AS7341_CHANNEL_480nm_F3);
    payload_raw.rawData.channelF4 = _sensor.getChannel(AS7341_CHANNEL_515nm_F4);
    payload_raw.rawData.channelF5 = _sensor.getChannel(AS7341_CHANNEL_555nm_F5);
    payload_raw.rawData.channelF6 = _sensor.getChannel(AS7341_CHANNEL_590nm_F6);
    payload_raw.rawData.channelF7 = _sensor.getChannel(AS7341_CHANNEL_630nm_F7);
    payload_raw.rawData.channelF8 = _sensor.getChannel(AS7341_CHANNEL_680nm_F8);
    memcpy(buf, &payload_raw, sizeof(payload_raw));
    return sizeof(payload_raw);
  }

  // Convert raw sensor data to RGBW data
  // - red: uses 555nm (F5)
  // - green: uses 590nm (F6)
  // - blue: uses 480nm (F3)
  // - white: average of channels F1, F2, F4, and F7
  void convertToRGBW(AS7341Payload_t& payload) {
    uint16_t chF1 = _sensor.getChannel(AS7341_CHANNEL_415nm_F1);
    uint16_t chF2 = _sensor.getChannel(AS7341_CHANNEL_445nm_F2);
    uint16_t chF3 = _sensor.getChannel(AS7341_CHANNEL_480nm_F3);
    uint16_t chF4 = _sensor.getChannel(AS7341_CHANNEL_515nm_F4);
    uint16_t chF5 = _sensor.getChannel(AS7341_CHANNEL_555nm_F5);
    uint16_t chF6 = _sensor.getChannel(AS7341_CHANNEL_590nm_F6);
    uint16_t chF7 = _sensor.getChannel(AS7341_CHANNEL_630nm_F7);
    uint16_t chF8 = _sensor.getChannel(AS7341_CHANNEL_680nm_F8);

    payload.rgbwData.red = chF5;
    payload.rgbwData.green = chF6;
    payload.rgbwData.blue = chF3;
    payload.rgbwData.white = (chF1 + chF2 + chF4 + chF7) / 4;

    SerialLogger.verbose(
        "AS7341Module: RGBW Conversion - R: %d, G: %d, B: %d, W: %d",
        payload.rgbwData.red, payload.rgbwData.green, payload.rgbwData.blue,
        payload.rgbwData.white);
  }

  Adafruit_AS7341 _sensor;
  uint8_t _gain = 0;      // Default gain
  uint8_t _atime = 100;   // Default ATIME (1 unit = 2.78 ms)
  uint16_t _astep = 999;  // Default ASTEP (steps per ATIME unit)
  bool _rgbwConversion = false;
};

}  // namespace Builtin
}  // namespace Modules
