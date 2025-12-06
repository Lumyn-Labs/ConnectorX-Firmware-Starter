/**
 * @file Animation.h
 * @brief Animation types for custom LED animations
 *
 * This header provides the AnimationInstance structure for defining
 * custom animations that can be registered with the AnimationManager.
 *
 * Copyright (c) Lumyn Labs, Inc. All rights reserved.
 * Licensed under the Lumyn Labs SDK License.
 */

#pragma once

#include "Color.h"
#include <functional>
#include <string_view>
#include <cstdint>

namespace LumynLabs
{

  /**
   * @brief State counting mode for animations
   */
  enum class AnimationStateMode
  {
    /**
     * @brief Constant state count
     *
     * Animation has a fixed number of states regardless of LED count.
     * Use for animations like "blink" or "pulse" that cycle through
     * fixed states.
     */
    Constant = 0,

    /**
     * @brief LED count based state count
     *
     * Animation state count is based on LED count plus a base value.
     * Use for animations like "chase" or "wave" that travel across LEDs.
     */
    LedCount,
  };

  /**
   * @brief Animation frame callback
   *
   * Called for each animation frame to update LED states.
   *
   * @param strip Pointer to LED color array (modify this to set LED colors)
   * @param color The requested animation color
   * @param state Current animation state (0 to stateCount-1)
   * @param count Total number of LEDs in the strip
   * @return true if the updated colors should be pushed to hardware
   *
   * Example:
   * @code
   * bool myAnimation(Color* strip, Color color, uint16_t state, uint16_t count) {
   *   // Simple chase animation
   *   for (uint16_t i = 0; i < count; i++) {
   *     strip[i] = (i == state) ? color : Color::Black();
   *   }
   *   return true; // Show the update
   * }
   * @endcode
   */
  using AnimationFrameCallback = std::function<bool(Color *, Color, uint16_t, uint16_t)>;

  /**
   * @brief Animation definition
   *
   * Defines a custom animation that can be registered with AnimationManager.
   *
   * Example:
   * @code
   * AnimationInstance myChase = {
   *   .id = "MY_CHASE",
   *   .stateMode = AnimationStateMode::LedCount,
   *   .stateCount = 0,  // Will be LED count
   *   .defaultDelay = 50,
   *   .defaultColor = Color::Blue(),
   *   .cb = [](Color* strip, Color color, uint16_t state, uint16_t count) {
   *     for (uint16_t i = 0; i < count; i++) {
   *       strip[i] = (i == state) ? color : Color::Black();
   *     }
   *     return true;
   *   }
   * };
   *
   * // Register with animation manager
   * AnimationMngr.registerAnimation(myChase);
   * @endcode
   */
  struct AnimationInstance
  {
    /**
     * @brief Unique animation identifier
     *
     * Used to reference this animation by name.
     * Should be uppercase with underscores (e.g., "MY_ANIMATION").
     */
    std::string_view id;

    /**
     * @brief State counting mode
     *
     * Determines how stateCount is interpreted.
     */
    AnimationStateMode stateMode;

    /**
     * @brief Number of states in animation
     *
     * For Constant mode: Total number of states.
     * For LedCount mode: Added to LED count for total states.
     */
    uint16_t stateCount;

    /**
     * @brief Default delay between frames in milliseconds
     *
     * Can be overridden when setting the animation.
     */
    uint16_t defaultDelay;

    /**
     * @brief Default color
     *
     * Used when no color is specified.
     */
    Color defaultColor;

    /**
     * @brief Animation frame callback
     *
     * Called for each frame to update LED states.
     */
    AnimationFrameCallback cb;
  };

  /**
   * @brief No-op animation (turns LEDs off)
   */
  static const AnimationInstance NoAnimation = {
      .id = "NONE",
      .stateMode = AnimationStateMode::Constant,
      .stateCount = 1,
      .defaultDelay = 65535U,
      .defaultColor = Color::Black(),
      .cb = [](Color *, Color, uint16_t, uint16_t)
      { return false; }};

} // namespace LumynLabs
