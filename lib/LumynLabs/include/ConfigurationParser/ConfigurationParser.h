#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <SPI.h>

#include <memory>
#include <optional>
#include <string_view>

#include "Configuration.h"
#include "ConfigurationParser/ConfigurationValidator.h"
#include "ParseResult.h"
#include "lumyn/domain/module/ModuleInfo.h"

class ConfigurationParser {
 public:
  ConfigurationParser(fs::FS& fs);

  // ! delete MUST be called on the returned object
  std::optional<Configuration::CXv3Configuration*> parseConfiguration(
      std::string_view filename, Configuration::ParseResult* result) const;

  std::optional<std::unique_ptr<Configuration::CXv3Configuration>>
  parseConfigurationString(const std::string_view& json,
                           Configuration::ParseResult* result) const;

 private:
  std::optional<Configuration::CXv3Configuration*> validateAndParse(
      JsonDocument& doc, Configuration::ParseResult* result) const;

  Configuration::CXv3Configuration* docToConfig(
      ArduinoJson::JsonDocument& doc) const;

  void networkToConfig(JsonObjectConst obj,
                       Configuration::Network* network) const;

  Configuration::Channel channelToConfig(JsonObjectConst obj) const;

  void zoneToConfig(JsonObjectConst obj, Configuration::Zone* zone) const;

  std::vector<Configuration::AnimationSequence> animationSequencessToConfig(
      JsonArrayConst arr) const;

  void animationSequenceToConfig(
      JsonObjectConst obj, Configuration::AnimationSequence* sequence) const;

  std::vector<Configuration::Animation> animationsToConfig(
      JsonArrayConst arr) const;

  void animationsToConfig(JsonObjectConst obj,
                          Configuration::Animation* animation) const;

  std::vector<Configuration::Bitmap> bitmapsToConfig(JsonArrayConst arr) const;

  void bitmapToConfig(JsonObjectConst obj, Configuration::Bitmap* bitmap) const;

  std::vector<Configuration::Module> modulesToConfig(JsonArrayConst arr) const;

  void moduleToConfig(JsonObjectConst obj, Configuration::Module* module) const;

  std::vector<Configuration::AnimationGroup> animationGroupsToConfig(
      JsonArrayConst arr) const;

  void animationGroupToConfig(JsonObjectConst obj,
                              Configuration::AnimationGroup* group) const;

 private:
  fs::FS& _fs;
  std::unique_ptr<ConfigurationValidator> _validator;
};