/**
 * @file AnimationManager.h
 * @brief Public Animation Manager API wrapper
 *
 * Provides functions to register custom animations with the ConnectorX system.
 *
 * Copyright (c) Lumyn Labs, Inc. All rights reserved.
 * Licensed under the Lumyn Labs SDK License.
 */

#pragma once

#include <LumynLabs/Led/Animation.h>
#include <cstdint>

/**
 * @namespace LumynLabs
 * @brief Animation registration functions
 */
namespace LumynLabs
{

  /**
   * @brief Register a custom animation
   *
   * Example:
   * @code
   * static LumynLabs::AnimationInstance myAnim = { ... };
   *
   * void setup() {
   *   LumynLabs::System::init();
   *   LumynLabs::registerAnimation(myAnim);
   *   LumynLabs::System::initServices();
   * }
   * @endcode
   *
   * @param instance Animation definition
   * @return Animation ID for reference
   */
  uint16_t registerAnimation(const AnimationInstance &instance);

} // namespace LumynLabs
