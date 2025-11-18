#pragma once

#include <Adafruit_AS7341.h>

#include "Logger/Logger.h"
#include "Modules/BaseModule.h"

namespace Modules {
namespace Builtin {

struct __attribute__((packed)) AS7341Payload_t {
  uint8_t useRGBW;
  union {
    struct {
      uint16_t channelF1;
      uint16_t channelF2;
      uint16_t channelF3;
      uint16_t channelF4;
      uint16_t channelF5;
      uint16_t channelF6;
      uint16_t channelF7;
      uint16_t channelF8;
    } raw;
    struct {
      uint16_t red;
      uint16_t green;
      uint16_t blue;
      uint16_t white;
    } rgbw;
  } data;
};

class AS7341Module : public BaseModule<AS7341Payload_t> {
 public:
  AS7341Module(const Configuration::Module& config) : BaseModule(config) {
    parseConfig();
  }

 protected:
  ModuleError_t initModule() override {
    if (!_sensor.begin()) {
      SerialLogger.error("AS7341Module: Sensor initialization failed");
      return {
          .errorType = ModuleErrorType_t::InitError,
          .additionalInformation = {1, 0, 0, 0},
      };
    }

    _sensor.setGain(static_cast<as7341_gain_t>(_gain));
    _sensor.setATIME(_atime);
    _sensor.setASTEP(_astep);

    SerialLogger.info(
        "AS7341Module: Configured - Gain:%d, ATIME:%d, ASTEP:%d, RGBW:%s",
        _gain, _atime, _astep, _useRGBW ? "enabled" : "disabled");

    return {
        .errorType = ModuleErrorType_t::None,
        .additionalInformation = {0, 0, 0, 0},
    };
  }

  ModuleError_t readData(AS7341Payload_t* dataOut) override {
    if (!dataOut) {
      return {
          .errorType = ModuleErrorType_t::ReadError,
          .additionalInformation = {0xFF, 0, 0, 0},
      };
    }

    if (!_sensor.readAllChannels()) {
      SerialLogger.error("AS7341Module: Error reading channels");
      return {
          .errorType = ModuleErrorType_t::Timeout,
          .additionalInformation = {2, 0, 0, 0},
      };
    }

    dataOut->useRGBW = _useRGBW;

    if (_useRGBW) {
      readRGBW(dataOut);
    } else {
      readRaw(dataOut);
    }

    return {
        .errorType = ModuleErrorType_t::None,
        .additionalInformation = {0, 0, 0, 0},
    };
  }

 private:
  void parseConfig() {
    _gain = 0;
    _atime = 100;
    _astep = 999;
    _useRGBW = false;

    if (!config().customConfig) {
      return;
    }

    auto& cfg = *config().customConfig;

    if (cfg.containsKey("gain") && cfg["gain"]) {
      _gain = cfg["gain"].as<uint8_t>();
      SerialLogger.verbose("AS7341Module: gain=%d", _gain);
    }

    if (cfg.containsKey("atime") && cfg["atime"]) {
      _atime = cfg["atime"].as<uint8_t>();
      SerialLogger.verbose("AS7341Module: atime=%d", _atime);
    }

    if (cfg.containsKey("astep") && cfg["astep"]) {
      _astep = cfg["astep"].as<uint16_t>();
      SerialLogger.verbose("AS7341Module: astep=%d", _astep);
    }

    if (cfg.containsKey("useRGBW") && cfg["useRGBW"]) {
      _useRGBW = cfg["useRGBW"].as<bool>();
      SerialLogger.verbose("AS7341Module: useRGBW=%s",
                           _useRGBW ? "true" : "false");
    }
  }

  void readRaw(AS7341Payload_t* dataOut) {
    dataOut->data.raw.channelF1 = _sensor.getChannel(AS7341_CHANNEL_415nm_F1);
    dataOut->data.raw.channelF2 = _sensor.getChannel(AS7341_CHANNEL_445nm_F2);
    dataOut->data.raw.channelF3 = _sensor.getChannel(AS7341_CHANNEL_480nm_F3);
    dataOut->data.raw.channelF4 = _sensor.getChannel(AS7341_CHANNEL_515nm_F4);
    dataOut->data.raw.channelF5 = _sensor.getChannel(AS7341_CHANNEL_555nm_F5);
    dataOut->data.raw.channelF6 = _sensor.getChannel(AS7341_CHANNEL_590nm_F6);
    dataOut->data.raw.channelF7 = _sensor.getChannel(AS7341_CHANNEL_630nm_F7);
    dataOut->data.raw.channelF8 = _sensor.getChannel(AS7341_CHANNEL_680nm_F8);

    SerialLogger.verbose(
        "AS7341Module: Raw - F1:%u F2:%u F3:%u F4:%u F5:%u F6:%u F7:%u F8:%u",
        dataOut->data.raw.channelF1, dataOut->data.raw.channelF2,
        dataOut->data.raw.channelF3, dataOut->data.raw.channelF4,
        dataOut->data.raw.channelF5, dataOut->data.raw.channelF6,
        dataOut->data.raw.channelF7, dataOut->data.raw.channelF8);
  }

  void readRGBW(AS7341Payload_t* dataOut) {
    uint16_t chF1 = _sensor.getChannel(AS7341_CHANNEL_415nm_F1);
    uint16_t chF2 = _sensor.getChannel(AS7341_CHANNEL_445nm_F2);
    uint16_t chF3 = _sensor.getChannel(AS7341_CHANNEL_480nm_F3);
    uint16_t chF4 = _sensor.getChannel(AS7341_CHANNEL_515nm_F4);
    uint16_t chF5 = _sensor.getChannel(AS7341_CHANNEL_555nm_F5);
    uint16_t chF6 = _sensor.getChannel(AS7341_CHANNEL_590nm_F6);
    uint16_t chF7 = _sensor.getChannel(AS7341_CHANNEL_630nm_F7);

    dataOut->data.rgbw.red = chF5;
    dataOut->data.rgbw.green = chF6;
    dataOut->data.rgbw.blue = chF3;
    dataOut->data.rgbw.white = (chF1 + chF2 + chF4 + chF7) / 4;

    SerialLogger.verbose("AS7341Module: RGBW - R:%u G:%u B:%u W:%u",
                         dataOut->data.rgbw.red, dataOut->data.rgbw.green,
                         dataOut->data.rgbw.blue, dataOut->data.rgbw.white);
  }

  Adafruit_AS7341 _sensor;
  uint8_t _gain;
  uint8_t _atime;
  uint16_t _astep;
  bool _useRGBW;
};

}  // namespace Builtin
}  // namespace Modules