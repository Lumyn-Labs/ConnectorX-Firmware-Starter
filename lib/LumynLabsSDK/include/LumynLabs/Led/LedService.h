/**
 * @file LedService.h
 * @brief Public LED Service API for controlling LED animations and colors
 *
 * Provides functions to control LED zones, animations, bitmaps, and matrix text.
 * These functions are the public API for SDK users to control LED output.
 *
 * Copyright (c) Lumyn Labs, Inc. All rights reserved.
 * Licensed under the Lumyn Labs SDK License.
 */

#pragma once

#include "Color.h"
#include <cstdint>
#include <optional>
#include <string_view>

namespace LumynLabs
{
  namespace Led
  {

    /**
     * @brief Matrix text scroll direction
     */
    enum class ScrollDirection : uint8_t
    {
      Left = 0, ///< Scroll text from right to left
      Right     ///< Scroll text from left to right
    };

    /**
     * @brief Set animation on a zone
     *
     * @param zoneId Zone identifier string
     * @param animationId Animation identifier string
     * @param delay Animation delay in milliseconds
     * @param color Animation color
     * @param reversed Run animation in reverse
     * @param oneShot Run animation once then stop
     */
    void setAnimation(std::string_view zoneId, std::string_view animationId,
                      uint16_t delay, LumynLabs::Color color,
                      bool reversed = false, bool oneShot = false);

    /**
     * @brief Set animation sequence on a zone
     *
     * @param zoneId Zone identifier string
     * @param sequenceId Sequence identifier string
     */
    void setAnimationSequence(std::string_view zoneId, std::string_view sequenceId);

    /**
     * @brief Set animation on a group of zones
     *
     * @param groupId Group identifier string
     * @param animationId Animation identifier string
     * @param delay Animation delay in milliseconds
     * @param color Animation color
     * @param reversed Run animation in reverse
     * @param oneShot Run animation once then stop
     */
    void setAnimationGroup(std::string_view groupId, std::string_view animationId,
                           uint16_t delay, LumynLabs::Color color,
                           bool reversed = false, bool oneShot = false);

    /**
     * @brief Set animation sequence on a group of zones
     *
     * @param groupId Group identifier string
     * @param sequenceId Sequence identifier string
     */
    void setAnimationSequenceGroup(std::string_view groupId, std::string_view sequenceId);

    /**
     * @brief Set solid color on a zone
     *
     * @param zoneId Zone identifier string
     * @param color Color to set
     */
    void setColor(std::string_view zoneId, LumynLabs::Color color);

    /**
     * @brief Set solid color on a group of zones
     *
     * @param groupId Group identifier string
     * @param color Color to set
     */
    void setColorGroup(std::string_view groupId, LumynLabs::Color color);

    /**
     * @brief Set bitmap on a zone
     *
     * @param zoneId Zone identifier string
     * @param bitmapId Bitmap identifier string
     * @param color Optional color tint (nullopt for original bitmap colors)
     * @param oneShot Display bitmap once then stop
     */
    void setBitmap(std::string_view zoneId, std::string_view bitmapId,
                   std::optional<LumynLabs::Color> color = std::nullopt,
                   bool oneShot = false);

    /**
     * @brief Set bitmap on a group of zones
     *
     * @param groupId Group identifier string
     * @param bitmapId Bitmap identifier string
     * @param color Optional color tint (nullopt for original bitmap colors)
     * @param oneShot Display bitmap once then stop
     */
    void setBitmapGroup(std::string_view groupId, std::string_view bitmapId,
                        std::optional<LumynLabs::Color> color = std::nullopt,
                        bool oneShot = false);

    /**
     * @brief Set scrolling text on a matrix zone
     *
     * @param zoneId Zone identifier string (must be a matrix zone)
     * @param color Text color
     * @param direction Scroll direction
     * @param text Text to display (max 24 characters)
     * @param delay Scroll delay in milliseconds
     * @param oneShot Scroll text once then stop
     */
    void setMatrixText(std::string_view zoneId, LumynLabs::Color color,
                       ScrollDirection direction, std::string_view text,
                       uint16_t delay, bool oneShot = false);

    /**
     * @brief Set scrolling text on a group of matrix zones
     *
     * @param groupId Group identifier string
     * @param color Text color
     * @param direction Scroll direction
     * @param text Text to display (max 24 characters)
     * @param delay Scroll delay in milliseconds
     * @param oneShot Scroll text once then stop
     */
    void setMatrixTextGroup(std::string_view groupId, LumynLabs::Color color,
                            ScrollDirection direction, std::string_view text,
                            uint16_t delay, bool oneShot = false);

  } // namespace Led

} // namespace LumynLabs
