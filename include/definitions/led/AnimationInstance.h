#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include <functional>
#include <optional>
#include <string_view>

#include "ConfigurationParser/Configuration.h"

namespace Animation {
enum class StateMode {
  Constant = 0,
  // Adds a predefined number of states to the LED count for the total number of
  // states
  LedCount,
};

/**
 * @brief Animation callback definition for setting LED states
 *
 * @param CRGB* The manipulable array of strip colors
 * @param CRGB The color being requested
 * @param uint16_t The current state
 * @param uint16_t Total count of LEDs in the array
 *
 * @return true if the new array state should be shown (pushed) to the physical
 * strip
 */
typedef std::function<bool(CRGB*, CRGB, uint16_t, uint16_t)>
    AnimationFrameCallback;

struct AnimationInstance {
  std::string_view id;
  StateMode stateMode;
  uint16_t stateCount;
  uint16_t defaultDelay;
  Configuration::ActionColor defaultColor;
  AnimationFrameCallback cb;
};

static AnimationInstance None = {
    .id = "NONE",
    .stateMode = StateMode::Constant,
    .stateCount = 1,
    .defaultDelay = 65535U,
    .defaultColor = {0, 0, 0},
    .cb = [](CRGB* strip, CRGB color, uint16_t state, uint16_t count) {
      return false;
    }};
}  // namespace Animation