/**
 * @file LumynLabs.h
 * @brief Main SDK entry point for Lumyn Labs ConnectorX firmware development
 *
 * Include this header to access all public SDK APIs for your board variant.
 * Features are conditionally included based on your board's capabilities.
 *
 * Copyright (c) Lumyn Labs, Inc. All rights reserved.
 * Licensed under the Lumyn Labs SDK License.
 */

#pragma once

// Feature flags and variant info - generated per board variant
#include "LumynLabs/Features.h"

// Core types - always available
#include "LumynLabs/Eventing/EventType.h"
#include "LumynLabs/Eventing/Event.h"

// LED APIs - conditional on CX_FEATURE_LED
#if CX_FEATURE_LED
#include "LumynLabs/Led/Color.h"
#include "LumynLabs/Led/Animation.h"
#endif

// Module APIs - conditional on CX_FEATURE_MODULES
#if CX_FEATURE_MODULES
#include "LumynLabs/Modules/Module.h"
#include "LumynLabs/Modules/ModuleConfig.h"
#include "LumynLabs/Modules/ModuleError.h"
#include "LumynLabs/Modules/ModulePeripherals.h"
#include "LumynLabs/Modules/ModuleRegistration.h"
#endif

/**
 * @namespace LumynLabs
 * @brief Root namespace for Lumyn Labs SDK
 *
 * All public SDK types and functions are in this namespace.
 */
namespace LumynLabs
{

  /**
   * @brief Get SDK version string
   * @return Version string in format "major.minor.patch"
   */
  inline const char *getSDKVersion()
  {
    return CX_SDK_VERSION_STRING;
  }

  /**
   * @brief Get current board variant name
   * @return Variant identifier string (e.g., "connector-x-animate")
   */
  inline const char *getVariant()
  {
    return CX_SDK_VARIANT;
  }

#if CX_FEATURE_LED
  /**
   * @brief Get the number of LED channels on this variant
   * @return Number of LED channels
   */
  inline uint8_t getLedChannelCount()
  {
    return CX_LED_CHANNEL_COUNT;
  }
#endif

} // namespace LumynLabs
