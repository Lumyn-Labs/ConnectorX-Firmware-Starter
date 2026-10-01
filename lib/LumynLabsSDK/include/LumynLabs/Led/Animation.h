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
#include "lumyn/definitions/led/AnimationInstance.h"

namespace LumynLabs
{

  /**
   * @brief State counting mode for animations
   */
  using AnimationStateMode = lumyn::internal::Animation::StateMode;

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
  using AnimationFrameCallback = lumyn::internal::Animation::AnimationFrameCallback;

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
   * LumynLabs::registerAnimation(myChase);
   * @endcode
   */
  using AnimationInstance = lumyn::internal::Animation::AnimationInstance;

  /**
   * @brief No-op animation (turns LEDs off)
   */
  static const AnimationInstance &NoAnimation = lumyn::internal::Animation::None;

} // namespace LumynLabs
