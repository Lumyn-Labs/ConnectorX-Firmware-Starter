/**
 * @file Color.h
 * @brief RGB Color type for LED operations
 *
 * Copyright (c) Lumyn Labs, Inc. All rights reserved.
 * Licensed under the Lumyn Labs SDK License.
 */

#pragma once

#include "lumyn/domain/Color.h"

namespace LumynLabs
{

  /**
   * @brief RGB Color structure
   *
   * Used for setting LED colors and animation parameters. It has r, g and b
   * bytes, toHSV(), blend(), fromHSV(), and named colors such as
   * Color::Blue().
   */
  using Color = lumyn::led::Color;

} // namespace LumynLabs
