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
   * @brief One zone's running animation
   *
   * An effect is made for one zone's size and keeps whatever state it
   * needs. It moves in fixed ticks of its own time, kTicksPerSecond to the
   * second; a command's delay sets how fast that time runs
   * (speed = defaultDelay / delay), so an effect never reads a clock.
   *
   * - step() moves the effect on by one tick.
   * - draw(frame) draws the effect as it is now on a cleared frame: the
   *   zone's pixels row by row from the top left. A strip is one row.
   *
   * Its random numbers (rnd(), chance(), randomBelow()) are the zone's own,
   * so the same commands draw the same frames.
   */
  using lumyn::led::Effect;

  /**
   * @brief What an effect draws on: pixels, width, height, color
   */
  using lumyn::led::Frame;

  /// Ticks of an effect's own time in a second, at its default delay.
  using lumyn::led::kTicksPerSecond;

  /**
   * @brief Flags for AnimationInstance::flags, combined with |
   *
   * - kMatrixOnly: needs a matrix, at least two rows and two columns.
   * - kOwnsReverse: reversed changes the effect itself, rather than flipping
   *   its pixels.
   * - kEnds: played one-shot, it stops after one cycle. Others loop.
   * - kStill: its frame changes only when its color does.
   */
  using lumyn::led::AnimationFlag;
  using lumyn::led::kEnds;
  using lumyn::led::kMatrixOnly;
  using lumyn::led::kOwnsReverse;
  using lumyn::led::kStill;

  /// Makes an effect of type E for AnimationInstance::make. E is made with
  /// the zone's width and height, or with no arguments if it doesn't need
  /// them.
  using lumyn::led::makeEffect;

  /**
   * @brief Animation definition
   *
   * Defines a custom animation that can be registered with AnimationManager.
   *
   * Example:
   * @code
   * class MyChase : public LumynLabs::Effect {
   *  public:
   *   MyChase(uint16_t width, uint16_t height) : _width(width) {}
   *   void step() override { _head = (_head + 1) % (_width * 4); }
   *   void draw(LumynLabs::Frame& frame) override {
   *     for (int y = 0; y < frame.height; y++)
   *       frame.at(_head / 4, y) = frame.color;
   *   }
   *  private:
   *   int _width;
   *   int _head = 0;
   * };
   *
   * const LumynLabs::AnimationInstance myChase = {
   *   .id = "MY_CHASE",
   *   .defaultDelay = 50,
   *   .defaultColor = LumynLabs::Color(0, 0, 255),
   *   .cycleTicks = 120,  // one sequence step: 2 s at the default delay
   *   .flags = 0,
   *   .make = LumynLabs::makeEffect<MyChase>,
   * };
   *
   * LumynLabs::registerAnimation(myChase);
   * @endcode
   */
  using lumyn::led::AnimationInstance;

  /**
   * @brief No-op animation (turns LEDs off)
   */
  extern const AnimationInstance &NoAnimation;

} // namespace LumynLabs
