#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include "APDS9151.h"
#include "Logger/Logger.h"
#include "Modules/BaseModule.h"

namespace Modules {
namespace Builtin {

struct __attribute__((packed)) APDS9151Payload_t {
  uint8_t valid;
  uint32_t red;
  uint32_t green;
  uint32_t blue;
  uint32_t clear;
  uint16_t proximity;
  uint8_t proximityOverflow;
  float colorTemp;
  float illuminance;
};

class APDS9151Module : public BaseModule<APDS9151Payload_t> {
 public:
  APDS9151Module(const Configuration::Module& config)
      : BaseModule(config),
        _sensor(lumyn::sensors::APDS9151::DEFAULT_ADDRESS) {}

 protected:
  ModuleError_t initModule() override {
    if (!_sensor.begin(WireInstance)) {
      uint8_t partId = _sensor.getPartID();
      SerialLogger.error(
          "APDS9151Module: Failed to initialize sensor (PartID=0x%02X, "
          "expected 0x%02X)",
          partId, lumyn::sensors::APDS9151::PART_ID_VALUE);
      return {
          .errorType = ModuleErrorType_t::InitError,
          .additionalInformation = {1, partId, 0, 0},
      };
    }

    parseConfig();

    _sensor.setLSGain(_lsGain);
    _sensor.setLEDCurrent(_ledCurrent);
    _sensor.setPSPulses(_psPulses);
    _sensor.setLSMeasurementRate(_lsResolution, _lsMeasurementRate);
    _sensor.setPSMeasurementRate(_psResolution, _psMeasurementRate);

    if (_enablePSCancellation) {
      _sensor.setPSCancellation(_psDigitalCancel, _psAnalogCancel);
    }

    SerialLogger.info(
        "APDS9151Module: Configured - LSGain:%u, LED:%umA, PSPulses:%u, "
        "LSRate:%ums, PSRate:%ums",
        static_cast<uint8_t>(_lsGain), ledCurrentToMilliamps(_ledCurrent),
        _psPulses, measurementRateToMillis(_lsMeasurementRate),
        measurementRateToMillis(_psMeasurementRate));

    return {
        .errorType = ModuleErrorType_t::None,
        .additionalInformation = {0, 0, 0, 0},
    };
  }

  ModuleError_t readData(APDS9151Payload_t* dataOut) override {
    if (!dataOut) {
      return {
          .errorType = ModuleErrorType_t::ReadError,
          .additionalInformation = {0xFF, 0, 0, 0},
      };
    }

    // Read color data
    auto color = _sensor.getColor();
    auto proximityData = _sensor.getProximityData();

    // Validate data
    bool colorValid = validateColorData(color);
    bool proximityValid =
        !proximityData.overflow && proximityData.value <= _maxValidProximity;

    dataOut->valid = (colorValid && proximityValid) ? 1 : 0;
    dataOut->red = color.red;
    dataOut->green = color.green;
    dataOut->blue = color.blue;
    dataOut->clear = color.clear;
    dataOut->proximity = proximityData.value;
    dataOut->proximityOverflow = proximityData.overflow ? 1 : 0;

    if (colorValid) {
      dataOut->colorTemp = _sensor.getColorTemperature();
      dataOut->illuminance = _sensor.getIlluminance();
    } else {
      dataOut->colorTemp = 0.0f;
      dataOut->illuminance = 0.0f;
    }

    if (!dataOut->valid) {
      SerialLogger.verbose(
          "APDS9151Module: Invalid measurement - ColorValid=%d, "
          "ProxValid=%d, ProxOverflow=%d",
          colorValid, proximityValid, proximityData.overflow);
    } else {
      SerialLogger.verbose(
          "APDS9151Module: R=%u G=%u B=%u C=%u Prox=%u CCT=%.0fK Lux=%.2f",
          color.red, color.green, color.blue, color.clear, proximityData.value,
          dataOut->colorTemp, dataOut->illuminance);
    }

    return {
        .errorType = ModuleErrorType_t::None,
        .additionalInformation = {0, 0, 0, 0},
    };
  }

 private:
  void parseConfig() {
    // Defaults
    _lsGain = lumyn::sensors::APDS9151::Gain::X3;
    _ledCurrent = lumyn::sensors::APDS9151::LEDCurrent::MA_100;
    _psPulses = lumyn::sensors::APDS9151::DEFAULT_PS_PULSES;
    _lsResolution = lumyn::sensors::APDS9151::Resolution::BIT_20;
    _psResolution = lumyn::sensors::APDS9151::Resolution::BIT_20;
    _lsMeasurementRate = lumyn::sensors::APDS9151::MeasurementRate::MS_100;
    _psMeasurementRate = lumyn::sensors::APDS9151::MeasurementRate::MS_100;
    _enablePSCancellation = false;
    _psDigitalCancel = 0;
    _psAnalogCancel = 0;
    _minValidClear = _defaultMinValidClear;
    _maxValidProximity = lumyn::sensors::APDS9151::PS_MAX_VALUE;

    if (!config().customConfig) {
      return;
    }

    auto& cfg = *config().customConfig;

    if (cfg.containsKey("lsGain") && cfg["lsGain"]) {
      _lsGain = parseGain(cfg["lsGain"].as<String>());
      SerialLogger.verbose("APDS9151Module: lsGain=%u",
                           static_cast<uint8_t>(_lsGain));
    }

    if (cfg.containsKey("ledCurrent") && cfg["ledCurrent"]) {
      _ledCurrent = parseLEDCurrent(cfg["ledCurrent"].as<String>());
      SerialLogger.verbose("APDS9151Module: ledCurrent=%umA",
                           ledCurrentToMilliamps(_ledCurrent));
    }

    if (cfg.containsKey("psPulses") && cfg["psPulses"]) {
      _psPulses = cfg["psPulses"].as<uint8_t>();
      SerialLogger.verbose("APDS9151Module: psPulses=%u", _psPulses);
    }

    if (cfg.containsKey("lsResolution") && cfg["lsResolution"]) {
      _lsResolution = parseResolution(cfg["lsResolution"].as<String>());
      SerialLogger.verbose("APDS9151Module: lsResolution=%u",
                           static_cast<uint8_t>(_lsResolution));
    }

    if (cfg.containsKey("psResolution") && cfg["psResolution"]) {
      _psResolution = parseResolution(cfg["psResolution"].as<String>());
      SerialLogger.verbose("APDS9151Module: psResolution=%u",
                           static_cast<uint8_t>(_psResolution));
    }

    if (cfg.containsKey("lsMeasurementRate") && cfg["lsMeasurementRate"]) {
      _lsMeasurementRate =
          parseMeasurementRate(cfg["lsMeasurementRate"].as<String>());
      SerialLogger.verbose("APDS9151Module: lsMeasurementRate=%ums",
                           measurementRateToMillis(_lsMeasurementRate));
    }

    if (cfg.containsKey("psMeasurementRate") && cfg["psMeasurementRate"]) {
      _psMeasurementRate =
          parseMeasurementRate(cfg["psMeasurementRate"].as<String>());
      SerialLogger.verbose("APDS9151Module: psMeasurementRate=%ums",
                           measurementRateToMillis(_psMeasurementRate));
    }

    if (cfg.containsKey("psDigitalCancel") && cfg["psDigitalCancel"]) {
      _enablePSCancellation = true;
      _psDigitalCancel = cfg["psDigitalCancel"].as<uint16_t>();
      SerialLogger.verbose("APDS9151Module: psDigitalCancel=%u",
                           _psDigitalCancel);
    }

    if (cfg.containsKey("psAnalogCancel") && cfg["psAnalogCancel"]) {
      _enablePSCancellation = true;
      _psAnalogCancel = cfg["psAnalogCancel"].as<uint8_t>();
      SerialLogger.verbose("APDS9151Module: psAnalogCancel=%u",
                           _psAnalogCancel);
    }

    if (cfg.containsKey("minValidClear") && cfg["minValidClear"]) {
      _minValidClear = cfg["minValidClear"].as<uint32_t>();
      SerialLogger.verbose("APDS9151Module: minValidClear=%u", _minValidClear);
    }

    if (cfg.containsKey("maxValidProximity") && cfg["maxValidProximity"]) {
      _maxValidProximity = cfg["maxValidProximity"].as<uint16_t>();
      SerialLogger.verbose("APDS9151Module: maxValidProximity=%u",
                           _maxValidProximity);
    }
  }

  bool validateColorData(const lumyn::sensors::APDS9151::Color& color) {
    // Check for sensor saturation
    if (color.red >= lumyn::sensors::APDS9151::LS_MAX_VALUE ||
        color.green >= lumyn::sensors::APDS9151::LS_MAX_VALUE ||
        color.blue >= lumyn::sensors::APDS9151::LS_MAX_VALUE ||
        color.clear >= lumyn::sensors::APDS9151::LS_MAX_VALUE) {
      return false;
    }

    // Check minimum clear channel (ambient light)
    if (color.clear < _minValidClear) {
      return false;
    }

    return true;
  }

  lumyn::sensors::APDS9151::Gain parseGain(const String& str) {
    if (str == "X1") return lumyn::sensors::APDS9151::Gain::X1;
    if (str == "X3") return lumyn::sensors::APDS9151::Gain::X3;
    if (str == "X6") return lumyn::sensors::APDS9151::Gain::X6;
    if (str == "X9") return lumyn::sensors::APDS9151::Gain::X9;
    if (str == "X18") return lumyn::sensors::APDS9151::Gain::X18;
    return lumyn::sensors::APDS9151::Gain::X3;
  }

  lumyn::sensors::APDS9151::LEDCurrent parseLEDCurrent(const String& str) {
    if (str == "12.5" || str == "MA_12_5")
      return lumyn::sensors::APDS9151::LEDCurrent::MA_12_5;
    if (str == "25" || str == "MA_25")
      return lumyn::sensors::APDS9151::LEDCurrent::MA_25;
    if (str == "50" || str == "MA_50")
      return lumyn::sensors::APDS9151::LEDCurrent::MA_50;
    if (str == "100" || str == "MA_100")
      return lumyn::sensors::APDS9151::LEDCurrent::MA_100;
    return lumyn::sensors::APDS9151::LEDCurrent::MA_100;
  }

  lumyn::sensors::APDS9151::Resolution parseResolution(const String& str) {
    if (str == "20" || str == "BIT_20")
      return lumyn::sensors::APDS9151::Resolution::BIT_20;
    if (str == "19" || str == "BIT_19")
      return lumyn::sensors::APDS9151::Resolution::BIT_19;
    if (str == "18" || str == "BIT_18")
      return lumyn::sensors::APDS9151::Resolution::BIT_18;
    if (str == "17" || str == "BIT_17")
      return lumyn::sensors::APDS9151::Resolution::BIT_17;
    if (str == "16" || str == "BIT_16")
      return lumyn::sensors::APDS9151::Resolution::BIT_16;
    if (str == "13" || str == "BIT_13")
      return lumyn::sensors::APDS9151::Resolution::BIT_13;
    return lumyn::sensors::APDS9151::Resolution::BIT_20;
  }

  lumyn::sensors::APDS9151::MeasurementRate parseMeasurementRate(
      const String& str) {
    if (str == "25" || str == "MS_25")
      return lumyn::sensors::APDS9151::MeasurementRate::MS_25;
    if (str == "50" || str == "MS_50")
      return lumyn::sensors::APDS9151::MeasurementRate::MS_50;
    if (str == "100" || str == "MS_100")
      return lumyn::sensors::APDS9151::MeasurementRate::MS_100;
    if (str == "200" || str == "MS_200")
      return lumyn::sensors::APDS9151::MeasurementRate::MS_200;
    if (str == "500" || str == "MS_500")
      return lumyn::sensors::APDS9151::MeasurementRate::MS_500;
    if (str == "1000" || str == "MS_1000")
      return lumyn::sensors::APDS9151::MeasurementRate::MS_1000;
    if (str == "2000" || str == "MS_2000")
      return lumyn::sensors::APDS9151::MeasurementRate::MS_2000;
    return lumyn::sensors::APDS9151::MeasurementRate::MS_100;
  }

  uint8_t ledCurrentToMilliamps(lumyn::sensors::APDS9151::LEDCurrent current) {
    switch (current) {
      case lumyn::sensors::APDS9151::LEDCurrent::MA_12_5:
        return 12;
      case lumyn::sensors::APDS9151::LEDCurrent::MA_25:
        return 25;
      case lumyn::sensors::APDS9151::LEDCurrent::MA_50:
        return 50;
      case lumyn::sensors::APDS9151::LEDCurrent::MA_100:
        return 100;
      default:
        return 0;
    }
  }

  uint16_t measurementRateToMillis(
      lumyn::sensors::APDS9151::MeasurementRate rate) {
    switch (rate) {
      case lumyn::sensors::APDS9151::MeasurementRate::MS_25:
        return 25;
      case lumyn::sensors::APDS9151::MeasurementRate::MS_50:
        return 50;
      case lumyn::sensors::APDS9151::MeasurementRate::MS_100:
        return 100;
      case lumyn::sensors::APDS9151::MeasurementRate::MS_200:
        return 200;
      case lumyn::sensors::APDS9151::MeasurementRate::MS_500:
        return 500;
      case lumyn::sensors::APDS9151::MeasurementRate::MS_1000:
        return 1000;
      case lumyn::sensors::APDS9151::MeasurementRate::MS_2000:
        return 2000;
      default:
        return 0;
    }
  }

  lumyn::sensors::APDS9151 _sensor;

  lumyn::sensors::APDS9151::Gain _lsGain;
  lumyn::sensors::APDS9151::LEDCurrent _ledCurrent;
  uint8_t _psPulses;
  lumyn::sensors::APDS9151::Resolution _lsResolution;
  lumyn::sensors::APDS9151::Resolution _psResolution;
  lumyn::sensors::APDS9151::MeasurementRate _lsMeasurementRate;
  lumyn::sensors::APDS9151::MeasurementRate _psMeasurementRate;

  bool _enablePSCancellation;
  uint16_t _psDigitalCancel;
  uint8_t _psAnalogCancel;

  uint32_t _minValidClear;
  uint16_t _maxValidProximity;

  static constexpr uint32_t _defaultMinValidClear = 10;
};

}  // namespace Builtin
}  // namespace Modules