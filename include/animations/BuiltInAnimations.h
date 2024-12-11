#pragma once

#include <Arduino.h>

#include <vector>

#include "constants/ColorConstants.h"
#include "definitions/led/AnimationInstance.h"

namespace Animation {
static AnimationInstance Fill = {
    .id = "Fill",
    .stateMode = StateMode::Constant,
    .stateCount = 1,
    .defaultDelay = 20000,
    .defaultColor = ColorConstants::defaultAnimationColor,
    .cb = [](CRGB* strip, CRGB color, uint16_t state, uint16_t count) {
      for (size_t i = 0; i < count; i++) {
        strip[i] = color;
      }

      return true;
    }};

static AnimationInstance Blink = {
    .id = "Blink",
    .stateMode = StateMode::Constant,
    .stateCount = 2,
    .defaultDelay = 1000,
    .defaultColor = ColorConstants::defaultAnimationColor,
    .cb = [](CRGB* strip, CRGB color, uint16_t state, uint16_t count) {
      switch (state) {
        case 0:
          for (size_t i = 0; i < count; i++) {
            strip[i] = color;
          }
          return true;

        case 1:
          for (size_t i = 0; i < count; i++) {
            strip[i] = 0;
          }
          return true;

        default:
          return false;
      }
    }};

static AnimationInstance Breathe = {
    .id = "Breathe",
    .stateMode = StateMode::Constant,
    .stateCount = 512,
    .defaultDelay = 5,
    .defaultColor = ColorConstants::defaultAnimationColor,
    .cb = [](CRGB* strip, CRGB color, uint16_t state, uint16_t count) {
      if (state > 255) {
        state = 511 - state;
      }
      for (size_t i = 0; i < count; i++) {
        strip[i] = color.scale8(state & 0xff);
      }

      return true;
    }};

static AnimationInstance RainbowRoll = {
    .id = "RainbowRoll",
    .stateMode = StateMode::Constant,
    .stateCount = 256,
    .defaultDelay = 10,
    .defaultColor = ColorConstants::defaultAnimationColor,
    .cb = [](CRGB* strip, CRGB color, uint16_t state, uint16_t count) {
      fill_rainbow_circular(strip, count, state);

      return true;
    }};

static AnimationInstance SineRoll = {
    .id = "SineRoll",
    .stateMode = StateMode::Constant,
    .stateCount = 60,
    .defaultDelay = 5,
    .defaultColor = ColorConstants::defaultAnimationColor,
    .cb = [](CRGB* strip, CRGB color, uint16_t state, uint16_t count) {
      for (uint16_t index = 0; index < count; index++) {
        double t1 = (2 * M_PI / 60) * (60 - state);
        double t2 = (1 / 60) * M_PI * index;
        double brightness = sin(t2 + t1) + 1;
        // Truncate all values < 0 to 0
        uint8_t quantizedBrightness = 255 * (brightness / 2);
        strip[index] = color.scale8(quantizedBrightness);
      }

      return true;
    }};

static AnimationInstance Chase = {
    .id = "Chase",
    .stateMode = StateMode::LedCount,
    .stateCount = 5,
    .defaultDelay = 25,
    .defaultColor = ColorConstants::defaultAnimationColor,
    .cb = [](CRGB* strip, CRGB color, uint16_t state, uint16_t count) {
      uint16_t startState = 5;
      uint16_t endState = (count + 5) - 1;
      uint16_t startIndex = state;
      uint16_t endIndex = (state + 5) - 1;

      for (uint16_t index = startState; index <= endState; index++) {
        if (index >= startIndex && index <= endIndex) {
          strip[index - startState] = color;
        } else {
          strip[index - startState] = 0;
        }
      }

      return true;
    }};

static AnimationInstance FadeIn = {
    .id = "FadeIn",
    .stateMode = StateMode::Constant,
    .stateCount = 256,
    .defaultDelay = 5,
    .defaultColor = ColorConstants::defaultAnimationColor,
    .cb = [](CRGB* strip, CRGB color, uint16_t state, uint16_t count) {
      for (size_t i = 0; i < count; i++) {
        strip[i] = color;
        strip[i].fadeLightBy(255 - state);
      }
      return true;
    }};

static AnimationInstance FadeOut = {
    .id = "FadeOut",
    .stateMode = StateMode::Constant,
    .stateCount = 256,
    .defaultDelay = 5,
    .defaultColor = ColorConstants::defaultAnimationColor,
    .cb = [](CRGB* strip, CRGB color, uint16_t state, uint16_t count) {
      for (size_t i = 0; i < count; i++) {
        strip[i] = color;
        strip[i].fadeLightBy(state);
      }
      return true;
    }};

static AnimationInstance RainbowCycle = {
    .id = "RainbowCycle",
    .stateMode = StateMode::Constant,
    .stateCount = 256,
    .defaultDelay = 10,
    .defaultColor = ColorConstants::defaultAnimationColor,
    .cb = [](CRGB* strip, CRGB color, uint16_t state, uint16_t count) {
      CHSV hueColor{state & 0xff, 255, 255};

      for (size_t i = 0; i < count; i++) {
        strip[i] = hueColor;
      }

      return true;
    }};

static AnimationInstance AlternateBreathe = {
    .id = "AlternateBreathe",
    .stateMode = StateMode::Constant,
    .stateCount = 1024,
    .defaultDelay = 10,
    .defaultColor = ColorConstants::defaultAnimationColor,
    .cb = [](CRGB* strip, CRGB color, uint16_t state, uint16_t count) {
      for (size_t i = 0; i < count; i++) {
        bool evenOdd = false;
        auto stateCpy = state;

        if (stateCpy > 511) {
          evenOdd = true;
          stateCpy = state - 512;
        }

        if (stateCpy > 255) {
          stateCpy = 511 - stateCpy;
        }

        float ratio = stateCpy / 256;
        // If the current LED is even and state < 512, then illuminate
        if (i % 2 == 0 && !evenOdd) {
          strip[i] = color;
          strip[i].fadeLightBy(256 - state);
        } else if (i % 2 == 1 && evenOdd) {
          strip[i] = color;
          strip[i].fadeLightBy(256 - state);
        } else {
          strip[i] = CRGB::Black;
        }
      }

      return true;
    }};

static AnimationInstance GrowingBreathe = {
    .id = "GrowingBreathe",
    .stateMode = StateMode::Constant,
    .stateCount = 256,
    .defaultDelay = 10,
    .defaultColor = ColorConstants::defaultAnimationColor,
    .cb = [](CRGB* strip, CRGB color, uint16_t state, uint16_t count) {
      float ratio = state / 256.0;
      uint16_t maxIndex = std::round(ratio * count);

      for (size_t i = 0; i < maxIndex; i++) {
        strip[i] = color;
        strip[i].fadeLightBy(256 - state);
      }

      for (size_t i = maxIndex; i < count; i++) {
        strip[i] = CRGB::Black;
      }

      return true;
    }};

static std::vector<AnimationInstance> BuiltInAnimations = {
    None,  Fill,   Blink,   Breathe,      RainbowRoll,      SineRoll,
    Chase, FadeIn, FadeOut, RainbowCycle, AlternateBreathe, GrowingBreathe};
}  // namespace Animation