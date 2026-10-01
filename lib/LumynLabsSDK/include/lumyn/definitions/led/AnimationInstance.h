#pragma once

#include <functional>
#include <optional>
#include <string_view>

#include "lumyn/domain/Color.h"

namespace lumyn::internal::Animation
{
  enum class StateMode
  {
    // Constant state mode is used for animations that do not change based on the
    // number of LEDs
    Constant = 0,
    // Adds a predefined number of states to the LED count for the total number of
    // states. This is used for animations that change based on the number of LEDs.
    LedCount,
  };

  /**
   * @brief Animation callback definition for setting LED states
   *
   * @param lumyn::led::Color* The manipulable array of strip colors
   * @param lumyn::led::Color The color being requested
   * @param uint16_t The current state
   * @param uint16_t Total count of LEDs in the array
   *
   * @return true if the new array state should be shown (pushed) to the physical
   * strip
   */
  typedef std::function<bool(lumyn::led::Color *, lumyn::led::Color, uint16_t, uint16_t)>
      AnimationFrameCallback;

  struct AnimationInstance
  {
    std::string_view id;
    StateMode stateMode;
    uint16_t stateCount;
    uint16_t defaultDelay;
    lumyn::led::Color defaultColor;
    AnimationFrameCallback cb;
  };

  static AnimationInstance None = {
      .id = "NONE",
      .stateMode = StateMode::Constant,
      .stateCount = 1,
      .defaultDelay = 65535U,
      .defaultColor = {0, 0, 0},
      .cb = [](lumyn::led::Color *strip, lumyn::led::Color color, uint16_t state, uint16_t count)
      {
        return false;
      }};
} // namespace lumyn::internal::Animation