#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include <memory>
#include <optional>
#include <string_view>

#include "ParseResult.h"
#include "Configuration.h"

class ConfigurationValidator {
 public:
  Configuration::ParseResult checkConfigurationValid(
      ArduinoJson::JsonDocument& doc) const;

  // Validation helpers
  // ============================================
  Configuration::ParseResult checkNetworkConfigurationValid(
      ArduinoJson::JsonObjectConst obj) const;

  Configuration::ParseResult checkChannelsConfigurationValid(
      ArduinoJson::JsonObjectConst obj) const;

  Configuration::ParseResult checkChannelConfigurationValid(
      ArduinoJson::JsonObjectConst obj, const std::string& key) const;

  Configuration::ParseResult checkChannelConfigurationValid(
      ArduinoJson::JsonObjectConst obj) const;

  Configuration::ParseResult checkZoneConfigurationValid(
      ArduinoJson::JsonObjectConst obj) const;

  Configuration::ParseResult checkAnimationSequenceConfigurationValid(
      ArduinoJson::JsonArrayConst arr) const;

  Configuration::ParseResult checkAnimationConfigurationValid(
      ArduinoJson::JsonArrayConst arr) const;

  Configuration::ParseResult checkBitmapConfigurationValid(
      ArduinoJson::JsonArrayConst arr) const;

  Configuration::ParseResult checkSensorConfigurationValid(
      ArduinoJson::JsonArrayConst arr) const;

  Configuration::ParseResult checkAnimationGroupConfigurationValid(
      ArduinoJson::JsonArrayConst arr) const;
};