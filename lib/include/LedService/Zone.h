#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "ConfigurationParser/configs/Zone.h"
#include "definitions/led/AnimationInstance.h"

class Zone {
 public:
  Zone(const Configuration::Zone&, CRGB*);

  inline const Configuration::Zone& config() const { return _config; }

  virtual bool update() = 0;

  inline bool shouldUpdateState() const {
    // Only update if enough delay has passed and pattern can be run again
    return (millis() - _lastUpdateMs >= _curDelay) && !(done());
  }

  inline bool done() const { return _isOneShot && _doneRunning; }

  inline void reset() {
    _curState = 0;
    _lastUpdateMs = millis();
    _doneRunning = false;
  }

  inline void setColor(CRGB color) { _curColor = color; }

  inline void setColor(Configuration::ActionColor color) {
    _curColor = CRGB(color.r, color.g, color.b);
  }

  inline const CRGB* readPixels() const { return _buf; }

  inline void setBuffer(uint8_t* buf, uint16_t length) {
    _doneRunning = true;

    for (size_t i = 0; i < length * 3; i += 3) {
      _buf[i] = CRGB{buf[i], buf[i + 1], buf[i + 2]};
    }
  }

  inline uint16_t readCurrentState() const { return _curState; }

  static inline uint16_t color16(CRGB c) {
    // TODO: Constants
    uint8_t r = c.r >> (8 - 5);
    uint8_t g = c.g >> (8 - 6);
    uint8_t b = c.b >> (8 - 5);

    return (r << 11) | (g << 6) | b;
  }

 protected:
  const Configuration::Zone& _config;
  uint16_t _curState;
  CRGB _curColor;
  uint32_t _lastUpdateMs;
  uint16_t _curDelay;
  volatile bool _doneRunning;
  bool _isOneShot;
  CRGB* _buf;
};