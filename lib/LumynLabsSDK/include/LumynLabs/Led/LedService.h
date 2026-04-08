/**
 * @file LedService.h
 * @brief Public LED Service API for controlling LED animations and colors
 *
 * Provides functions to control LED zones, animations, bitmaps, and matrix
 * text. These functions are the public API for SDK users to control LED output.
 *
 * Copyright (c) Lumyn Labs, Inc. All rights reserved.
 * Licensed under the Lumyn Labs SDK License.
 */

#pragma once

#include <cstdint>
#include <optional>
#include <string_view>

#include "Color.h"

namespace LumynLabs {
namespace Led {

// ── Matrix Text Enums ──────────────────────────────────────────────

/**
 * @brief Matrix text scroll direction
 */
enum class ScrollDirection : uint8_t {
  Left = 0,   ///< Scroll text from right to left
  Right = 1,  ///< Scroll text from left to right
  Up = 2,     ///< Scroll text upward
  Down = 3,   ///< Scroll text downward
  None = 4    ///< No scrolling (use TextAlign for positioning)
};

/**
 * @brief Matrix text font selection
 *
 * Choose from built-in pixel fonts for small matrices, or FreeFont variants
 * at 9pt, 12pt, 18pt, and 24pt for larger displays.
 */
enum class TextFont : uint8_t {
  // Built-in / tiny fonts (best for 8×8 to 16×16 matrices)
  Builtin = 0,    ///< Default 5×7 built-in font
  Tiny3x3 = 1,    ///< 3×3 pixel font (extremely small)
  Picopixel = 2,  ///< 3×5 tiny font
  TomThumb = 3,   ///< 3×5 readable monospace font
  Org01 = 4,      ///< 5×6 stylised pixel font

  // 9pt (good for 16×16 to 32×32 matrices)
  FreeMono9 = 10,
  FreeMonoBold9 = 11,
  FreeSans9 = 12,
  FreeSansBold9 = 13,
  FreeSerif9 = 14,
  FreeSerifBold9 = 15,

  // 12pt (good for 32×32+ matrices)
  FreeMono12 = 20,
  FreeMonoBold12 = 21,
  FreeSans12 = 22,
  FreeSansBold12 = 23,
  FreeSerif12 = 24,
  FreeSerifBold12 = 25,

  // 18pt (large displays)
  FreeMono18 = 30,
  FreeMonoBold18 = 31,
  FreeSans18 = 32,
  FreeSansBold18 = 33,
  FreeSerif18 = 34,
  FreeSerifBold18 = 35,

  // 24pt (very large displays)
  FreeMono24 = 40,
  FreeMonoBold24 = 41,
  FreeSans24 = 42,
  FreeSansBold24 = 43,
  FreeSerif24 = 44,
  FreeSerifBold24 = 45,
};

/**
 * @brief Matrix text horizontal alignment (used when scrolling is disabled)
 */
enum class TextAlign : uint8_t {
  Left = 0,
  Center = 1,
  Right = 2,
};

/**
 * @brief Matrix text display flags
 */
struct TextFlags {
  bool smoothScroll =
      false;  ///< Pixel-by-pixel scrolling (vs character-by-character)
  bool showBackground = false;  ///< Render bgColor behind text (vs transparent)
  bool pingPong = false;        ///< Bounce back and forth instead of wrapping
  bool noScroll = false;        ///< Static text (use TextAlign for positioning)
};

// ── Animation ──────────────────────────────────────────────────────

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
                  uint16_t delay, LumynLabs::Color color, bool reversed = false,
                  bool oneShot = false);

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
void setAnimationSequenceGroup(std::string_view groupId,
                               std::string_view sequenceId);

// ── Color ──────────────────────────────────────────────────────────

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

// ── Bitmap ─────────────────────────────────────────────────────────

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

// ── Matrix Text ────────────────────────────────────────────────────

/**
 * @brief Set scrolling text on a matrix zone (simple)
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
 * @brief Set scrolling text on a matrix zone (advanced)
 *
 * Extended overload that supports background color, custom fonts,
 * text alignment, display flags, and vertical offset.
 *
 * @param zoneId Zone identifier string (must be a matrix zone)
 * @param color Text color
 * @param direction Scroll direction
 * @param text Text to display (max 24 characters)
 * @param delay Scroll delay in milliseconds
 * @param oneShot Scroll text once then stop
 * @param bgColor Background color (used when TextFlags::showBackground is set)
 * @param font Font to render with
 * @param align Horizontal alignment (used when TextFlags::noScroll is set)
 * @param flags Display and scrolling options
 * @param yOffset Signed vertical pixel offset for fine positioning
 */
void setMatrixText(std::string_view zoneId, LumynLabs::Color color,
                   ScrollDirection direction, std::string_view text,
                   uint16_t delay, bool oneShot, LumynLabs::Color bgColor,
                   TextFont font, TextAlign align, TextFlags flags,
                   int8_t yOffset);

/**
 * @brief Set scrolling text on a group of matrix zones (simple)
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

/**
 * @brief Set scrolling text on a group of matrix zones (advanced)
 *
 * Extended overload that supports background color, custom fonts,
 * text alignment, display flags, and vertical offset.
 *
 * @param groupId Group identifier string
 * @param color Text color
 * @param direction Scroll direction
 * @param text Text to display (max 24 characters)
 * @param delay Scroll delay in milliseconds
 * @param oneShot Scroll text once then stop
 * @param bgColor Background color (used when TextFlags::showBackground is set)
 * @param font Font to render with
 * @param align Horizontal alignment (used when TextFlags::noScroll is set)
 * @param flags Display and scrolling options
 * @param yOffset Signed vertical pixel offset for fine positioning
 */
void setMatrixTextGroup(std::string_view groupId, LumynLabs::Color color,
                        ScrollDirection direction, std::string_view text,
                        uint16_t delay, bool oneShot, LumynLabs::Color bgColor,
                        TextFont font, TextAlign align, TextFlags flags,
                        int8_t yOffset);

// ── Direct Buffer ──────────────────────────────────────────────────

/**
 * @brief Set raw RGB pixel data directly on a zone
 *
 * Writes raw RGB bytes to a zone's LED buffer, giving full per-pixel
 * control. This is the on-device equivalent of the SDK's DirectLEDBuffer
 * class
 *
 * The buffer is expected to be packed RGB (3 bytes per LED: R, G, B)
 * with 4-byte padding. Use paddedBufferSize() to calculate the required
 * buffer size.
 *
 * @param zoneId Zone identifier string
 * @param data Pointer to raw RGB byte buffer (must be 4-byte padded)
 * @param length Number of bytes in the buffer (use paddedBufferSize)
 * @return true on success, false if the zone is invalid or the buffer
 *         size does not match the zone's expected padded size
 */
bool setZoneBuffer(std::string_view zoneId, const uint8_t* data,
                   uint16_t length);

/**
 * @brief Calculate the 4-byte-padded buffer size for a given LED count
 *
 * The firmware expects direct buffers to be padded to 4-byte alignment.
 * Use this helper to determine the correct buffer size to allocate.
 *
 * @param ledCount Number of LEDs in the zone
 * @return Size in bytes of the padded buffer (>= ledCount * 3)
 */
constexpr uint16_t paddedBufferSize(uint16_t ledCount) {
  uint16_t byteCount = ledCount * 3;
  uint16_t remainder = byteCount % 4;
  return remainder == 0 ? byteCount
                        : static_cast<uint16_t>(byteCount + (4 - remainder));
}

}  // namespace Led

}  // namespace LumynLabs
