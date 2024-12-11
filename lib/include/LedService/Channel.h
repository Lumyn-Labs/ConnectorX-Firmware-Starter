#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include <map>
#include <memory>

#include "ChannelGetter.h"
#include "ConfigurationParser/configs/Channel.h"
#include "FileAccess/FileService.h"
#include "LedService/Zone.h"
#include "constants/board_variant.h"

// ? Can each Channel be its own FreeRTOS Task?
class Channel {
 public:
  Channel(const Configuration::Channel& config);

  void init(LEDChannelPort port, uint16_t totalLedCount);

  inline const Configuration::Channel& config() const { return _config; }

  inline const std::vector<std::shared_ptr<Zone>>& getZones() const {
    return _zones;
  }

  void update(bool force = false);

  void setZoneBuffer(Zone& zone, uint8_t* buf, uint16_t zoneLength);

  CLEDController* strip() const { return _strip; }
  CRGB* leds() const { return _leds; }

  static uint16_t getZoneSize(const Configuration::Zone& zone) {
    if (zone.type == Configuration::ZoneType::Strip) {
      return zone.strip.length;
    } else if (zone.type == Configuration::ZoneType::Matrix) {
      return zone.matrix.rows * zone.matrix.cols;
    }

    return 0;
  }

 private:
  static uint8_t getAutoDimBrightness(uint16_t length) {
    double unclampedBrightness = (CX_BOARD_INFO_REGULATOR_OUTPUT_MA /
                                  (length * CX_BOARD_LED_POWER_DRAW_MA)) *
                                 255;

    return std::clamp(static_cast<int>(unclampedBrightness), 0, 255);
  }

  void addZones(const std::vector<Configuration::Zone>& zones,
                uint16_t offset = 0);

  const Configuration::Channel& _config;
  std::vector<std::shared_ptr<Zone>> _zones;
  CRGB* _leds;
  CLEDController* _strip;
  uint8_t _brightness;
};