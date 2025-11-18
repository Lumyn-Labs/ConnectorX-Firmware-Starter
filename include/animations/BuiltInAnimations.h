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
static AnimationInstance Comet = {
    .id = "Comet",
    .stateMode = StateMode::LedCount,
    .stateCount = 1,
    .defaultDelay = 20,
    .defaultColor = ColorConstants::defaultAnimationColor,
    .cb = [](CRGB* strip, CRGB color, uint16_t state, uint16_t count) {
      const uint8_t tailLength = 8;

      for (uint16_t i = 0; i < count; i++) {
        strip[i] = CRGB::Black;
      }

      strip[state % count] = color;

      for (uint8_t t = 1; t <= tailLength; t++) {
        int16_t tailPos = (state - t);
        if (tailPos < 0) tailPos += count;

        uint8_t brightness = 255 * pow(0.7, t);
        strip[tailPos % count] = color.scale8(brightness);
      }

      return true;
    }};

static AnimationInstance Sparkle = {
    .id = "Sparkle",
    .stateMode = StateMode::Constant,
    .stateCount = 100,
    .defaultDelay = 150,
    .defaultColor = ColorConstants::defaultAnimationColor,
    .cb = [](CRGB* strip, CRGB color, uint16_t state, uint16_t count) {
      for (size_t i = 0; i < count; i++) {
        strip[i] = color.scale8(51);  // 20% background

        if (random8() < 8)  // ~3% chance
        {
          uint8_t sparkleIntensity = 153 + random8(102);  // 60-100%
          strip[i] = color.scale8(sparkleIntensity);
        }
      }

      return true;
    }};

static AnimationInstance Fire = {
    .id = "Fire",
    .stateMode = StateMode::Constant,
    .stateCount = 256,
    .defaultDelay = 15,
    .defaultColor = ColorConstants::defaultAnimationColor,
    .cb = [](CRGB* strip, CRGB color, uint16_t state, uint16_t count) {
      static uint8_t heat[256];

      for (uint16_t i = 0; i < count; i++) {
        heat[i] = qsub8(heat[i], random8(0, ((55 * 10) / count) + 2));
      }

      for (uint16_t k = count - 1; k >= 2; k--) {
        heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
      }

      if (random8() < 120) {
        uint16_t y = random8(7);
        heat[y] = qadd8(heat[y], random8(160, 255));
      }

      for (uint16_t j = 0; j < count; j++) {
        uint8_t t192 = scale8(heat[j], 191);
        uint8_t heatramp = t192 & 0x3F;
        heatramp <<= 2;

        CRGB fireColor;
        if (t192 & 0x80) {
          fireColor = CRGB(255, 255, heatramp);
        } else if (t192 & 0x40) {
          fireColor = CRGB(255, heatramp, 0);
        } else {
          fireColor = CRGB(heatramp, 0, 0);
        }

        strip[j] = fireColor.lerp8(color, 128);  // 50/50 blend
      }

      return true;
    }};

static AnimationInstance Scanner = {
    .id = "Scanner",
    .stateMode = StateMode::LedCount,
    .stateCount = 2,
    .defaultDelay = 30,
    .defaultColor = ColorConstants::defaultAnimationColor,
    .cb = [](CRGB* strip, CRGB color, uint16_t state, uint16_t count) {
      uint16_t totalStates = (count - 1) * 2;
      uint16_t pos = state % totalStates;

      if (pos >= count) {
        pos = totalStates - pos;
      }

      for (uint16_t i = 0; i < count; i++) {
        strip[i] = CRGB::Black;
      }

      const uint8_t eyeSize = 3;
      for (uint8_t i = 0; i < eyeSize; i++) {
        int16_t p = pos - (eyeSize - 1) / 2 + i;
        if (p >= 0 && p < count) {
          uint8_t brightness = 255 - (abs(i - 1) * 100);
          strip[p] = color.scale8(brightness);
        }
      }

      return true;
    }};

static AnimationInstance TheaterChase = {
    .id = "TheaterChase",
    .stateMode = StateMode::Constant,
    .stateCount = 3,
    .defaultDelay = 150,
    .defaultColor = ColorConstants::defaultAnimationColor,
    .cb = [](CRGB* strip, CRGB color, uint16_t state, uint16_t count) {
      for (uint16_t i = 0; i < count; i++) {
        if ((i + state) % 3 == 0) {
          strip[i] = color;
        } else {
          strip[i] = CRGB::Black;
        }
      }

      return true;
    }};

static AnimationInstance Twinkle = {
    .id = "Twinkle",
    .stateMode = StateMode::Constant,
    .stateCount = 256,
    .defaultDelay = 20,
    .defaultColor = ColorConstants::defaultAnimationColor,
    .cb = [](CRGB* strip, CRGB color, uint16_t state, uint16_t count) {
      static uint8_t brightness[256];
      static uint8_t direction[256];

      for (uint16_t i = 0; i < count; i++) {
        if (random8() < 5) {
          brightness[i] = 255;
          direction[i] = 1;
        }

        if (direction[i] == 1) {
          brightness[i] = qsub8(brightness[i], 8);
          if (brightness[i] < 20) {
            brightness[i] = 0;
            direction[i] = 0;
          }
        }

        strip[i] = color.scale8(brightness[i]);
      }

      return true;
    }};

static AnimationInstance Meteor = {
    .id = "Meteor",
    .stateMode = StateMode::LedCount,
    .stateCount = 1,
    .defaultDelay = 30,
    .defaultColor = ColorConstants::defaultAnimationColor,
    .cb = [](CRGB* strip, CRGB color, uint16_t state, uint16_t count) {
      const uint8_t meteorSize = 5;
      const uint8_t meteorTrailDecay = 64;

      for (uint16_t i = 0; i < count; i++) {
        if (random8() > 128) {
          strip[i].fadeLightBy(meteorTrailDecay);
        }
      }

      for (uint8_t j = 0; j < meteorSize; j++) {
        int16_t pos = state - j;
        if (pos >= 0 && pos < count) {
          strip[pos] = color;
        }
      }

      return true;
    }};

static AnimationInstance Wave = {
    .id = "Wave",
    .stateMode = StateMode::Constant,
    .stateCount = 360,
    .defaultDelay = 10,
    .defaultColor = ColorConstants::defaultAnimationColor,
    .cb = [](CRGB* strip, CRGB color, uint16_t state, uint16_t count) {
      const float waveCount = 2.0;

      for (uint16_t i = 0; i < count; i++) {
        float angle =
            (state * M_PI / 180.0) + (i * waveCount * 2 * M_PI / count);
        float brightness = (sin(angle) + 1.0) / 2.0;

        strip[i] = color.scale8(brightness * 255);
      }

      return true;
    }};

static AnimationInstance Pulse = {
    .id = "Pulse",
    .stateMode = StateMode::Constant,
    .stateCount = 256,
    .defaultDelay = 8,
    .defaultColor = ColorConstants::defaultAnimationColor,
    .cb = [](CRGB* strip, CRGB color, uint16_t state, uint16_t count) {
      uint16_t center = count / 2;
      uint16_t maxDistance = center;

      for (uint16_t i = 0; i < count; i++) {
        uint16_t distance = abs((int16_t)i - (int16_t)center);

        int16_t ringPos = state - (distance * 256 / maxDistance);
        if (ringPos < 0) ringPos += 256;

        uint8_t brightness = (sin8(ringPos) + 255) / 2;
        strip[i] = color.scale8(brightness);
      }

      return true;
    }};

static AnimationInstance Larson = {
    .id = "Larson",
    .stateMode = StateMode::LedCount,
    .stateCount = 2,
    .defaultDelay = 20,
    .defaultColor = ColorConstants::defaultAnimationColor,
    .cb = [](CRGB* strip, CRGB color, uint16_t state, uint16_t count) {
      const uint16_t totalStates = (count - 1) * 2;
      uint16_t pos = state % totalStates;

      if (pos >= count) {
        pos = totalStates - pos;
      }

      for (uint16_t i = 0; i < count; i++) {
        const float distance = abs((int16_t)i - (int16_t)pos);
        const float brightness = exp(-distance * distance / 8.0);
        strip[i] = color.scale8(brightness * 255);
      }

      return true;
    }};

static AnimationInstance Ripple = {
    .id = "Ripple",
    .stateMode = StateMode::Constant,
    .stateCount = 360,
    .defaultDelay = 25,
    .defaultColor = ColorConstants::defaultAnimationColor,
    .cb = [](CRGB* strip, CRGB color, uint16_t state, uint16_t count) {
      const uint16_t center = count / 2;

      for (uint16_t i = 0; i < count; i++) {
        strip[i] = color.scale8(25);  // 10% background
      }

      const uint8_t ringWidth = 3;
      const uint8_t ringSpeed = 2;

      for (uint8_t ring = 0; ring < 2; ring++) {
        const uint16_t ringPos =
            ((state + ring * 180) * ringSpeed) % (count * 2);

        for (uint16_t i = 0; i < count; i++) {
          const uint16_t distance = abs((int16_t)i - (int16_t)center);

          if (abs((int16_t)distance - (int16_t)(ringPos / 2)) < ringWidth) {
            const float falloff =
                1.0 - (float)abs((int16_t)distance - (int16_t)(ringPos / 2)) /
                          ringWidth;
            const uint8_t currentBrightness =
                (strip[i].r > 0) ? (strip[i].r * 255 / color.r) : 0;
            const uint8_t newBrightness =
                max((uint8_t)(falloff * 255), currentBrightness);

            strip[i] = color.scale8(newBrightness);
          }
        }
      }

      return true;
    }};

static AnimationInstance Confetti = {
    .id = "Confetti",
    .stateMode = StateMode::Constant,
    .stateCount = 200,
    .defaultDelay = 30,
    .defaultColor = ColorConstants::defaultAnimationColor,
    .cb = [](CRGB* strip, CRGB color, uint16_t state, uint16_t count) {
      for (uint16_t i = 0; i < count; i++) {
        strip[i].fadeLightBy(20);
      }

      uint8_t confettiCount = min(count / 3, 20);
      for (uint8_t i = 0; i < confettiCount; i++) {
        if (random8() < 30) {
          uint16_t pos = random16(count);
          uint8_t hue = random8();
          uint8_t brightness = 179 + random8(76);  // 70-100%
          strip[pos] = CHSV(hue, 255, brightness);
        }
      }

      return true;
    }};

static AnimationInstance Lava = {
    .id = "Lava",
    .stateMode = StateMode::Constant,
    .stateCount = 360,
    .defaultDelay = 50,
    .defaultColor = Configuration::ActionColor(255, 80, 0),
    .cb = [](CRGB* strip, CRGB color, uint16_t state, uint16_t count) {
      for (uint16_t i = 0; i < count; i++) {
        const float wave1 = sin(((state * 2 + i * 15) * M_PI) / 180.0);
        const float wave2 = sin(((state * 3 + i * 10) * M_PI) / 180.0);
        const float wave3 = sin(((state * 1.5 + i * 20) * M_PI) / 180.0);

        const float combined = (wave1 * 0.5 + wave2 * 0.3 + wave3 * 0.2);
        const float brightness = (combined + 1.0) / 2.0;

        const float finalBrightness = pow(brightness, 0.7);
        const float warmth = 0.7 + finalBrightness * 0.3;

        strip[i] = CRGB(color.r * warmth * finalBrightness,
                        color.g * finalBrightness * 0.6,
                        color.b * finalBrightness * 0.3);
      }

      return true;
    }};

static AnimationInstance Plasma = {
    .id = "Plasma",
    .stateMode = StateMode::Constant,
    .stateCount = 360,
    .defaultDelay = 20,
    .defaultColor = ColorConstants::defaultAnimationColor,
    .cb = [](CRGB* strip, CRGB color, uint16_t state, uint16_t count) {
      // Convert RGB to HSV
      uint8_t maxVal = max(max(color.r, color.g), color.b);
      uint8_t minVal = min(min(color.r, color.g), color.b);
      uint8_t delta = maxVal - minVal;

      uint8_t baseHue = 0;
      if (delta != 0) {
        if (maxVal == color.r) {
          baseHue = 43 * ((color.g - color.b) / delta);
        } else if (maxVal == color.g) {
          baseHue = 85 + 43 * ((color.b - color.r) / delta);
        } else {
          baseHue = 171 + 43 * ((color.r - color.g) / delta);
        }
      }

      uint8_t baseSat = (maxVal == 0) ? 0 : (255 * delta / maxVal);

      for (uint16_t i = 0; i < count; i++) {
        const float t = state * 0.04;
        const float x = (float)i / count;

        const float plasma =
            sin(x * 4 + t) + sin(x * 7 - t * 1.3) + sin(x * 11 + t * 0.7);

        const int16_t hueShift = ((plasma + 3.0) / 6.0) * 180 - 90;
        const uint8_t hue = (baseHue + hueShift + 256) % 256;

        const uint8_t saturation = constrain(baseSat + plasma * 8, 128, 255);
        const uint8_t value = 140 + (sin(plasma * 1.5) + 1.0) * 57;

        strip[i] = CHSV(hue, saturation, value);
      }

      return true;
    }};

static AnimationInstance Heartbeat = {
    .id = "Heartbeat",
    .stateMode = StateMode::Constant,
    .stateCount = 60,
    .defaultDelay = 30,
    .defaultColor = Configuration::ActionColor{255, 0, 0},
    .cb = [](CRGB* strip, CRGB color, uint16_t state, uint16_t count) {
      const uint8_t cycle = state % 60;
      float brightness = 0;

      if (cycle < 8) {
        brightness = sin((cycle / 8.0) * M_PI);
      } else if (cycle >= 12 && cycle < 18) {
        brightness = sin(((cycle - 12) / 6.0) * M_PI) * 0.8;
      }

      for (uint16_t i = 0; i < count; i++) {
        strip[i] = color.scale8(brightness * 255);
      }

      return true;
    }};

static std::vector<AnimationInstance> BuiltInAnimations = {
    None,   Fill,    Blink,    Breathe,      RainbowRoll,      SineRoll,
    Chase,  FadeIn,  FadeOut,  RainbowCycle, AlternateBreathe, GrowingBreathe,
    Comet,  Sparkle, Fire,     Scanner,      TheaterChase,     Twinkle,
    Meteor, Wave,    Pulse,    Larson,       Ripple,           Confetti,
    Lava,   Plasma,  Heartbeat};
}  // namespace Animation