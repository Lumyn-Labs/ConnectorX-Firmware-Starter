/**
 * @file Color.h
 * @brief RGB Color type for LED operations
 *
 * Copyright (c) Lumyn Labs, Inc. All rights reserved.
 * Licensed under the Lumyn Labs SDK License.
 */

#pragma once

#include <cstdint>

namespace LumynLabs
{

  /**
   * @brief RGB Color structure
   *
   * Used for setting LED colors and animation parameters.
   */
  struct Color
  {
    uint8_t r; ///< Red component (0-255)
    uint8_t g; ///< Green component (0-255)
    uint8_t b; ///< Blue component (0-255)

    constexpr Color() : r(0), g(0), b(0) {}
    constexpr Color(uint8_t red, uint8_t green, uint8_t blue)
        : r(red), g(green), b(blue) {}

    constexpr bool operator==(const Color &other) const
    {
      return r == other.r && g == other.g && b == other.b;
    }

    constexpr bool operator!=(const Color &other) const
    {
      return !(*this == other);
    }

    /**
     * @brief Convert RGB to HSV
     * @param hue Output hue (0-255)
     * @param sat Output saturation (0-255)
     * @param val Output value (0-255)
     */
    void toHSV(uint8_t &hue, uint8_t &sat, uint8_t &val) const
    {
      uint8_t maxVal = (r > g) ? ((r > b) ? r : b) : ((g > b) ? g : b);
      uint8_t minVal = (r < g) ? ((r < b) ? r : b) : ((g < b) ? g : b);
      uint8_t delta = maxVal - minVal;

      val = maxVal;

      if (delta == 0)
      {
        hue = 0;
        sat = 0;
      }
      else
      {
        sat = (255 * delta) / maxVal;

        if (maxVal == r)
        {
          hue = 43 * ((g - b) / delta);
        }
        else if (maxVal == g)
        {
          hue = 85 + 43 * ((b - r) / delta);
        }
        else
        {
          hue = 171 + 43 * ((r - g) / delta);
        }
      }
    }

    /**
     * @brief Blend with another color
     * @param other Color to blend with
     * @param amount Blend amount (0 = all this, 255 = all other)
     * @return Blended color
     */
    constexpr Color blend(const Color &other, uint8_t amount) const
    {
      return Color(r + (((int16_t)other.r - r) * amount) / 255,
                   g + (((int16_t)other.g - g) * amount) / 255,
                   b + (((int16_t)other.b - b) * amount) / 255);
    }

    /**
     * @brief Create color from HSV values
     * @param hue Hue (0-255)
     * @param sat Saturation (0-255)
     * @param val Value (0-255)
     * @return Color
     */
    static Color fromHSV(uint8_t hue, uint8_t sat, uint8_t val)
    {
      if (sat == 0)
      {
        return Color(val, val, val);
      }

      uint8_t region = hue / 43;
      uint8_t remainder = (hue - (region * 43)) * 6;
      uint8_t p = (val * (255 - sat)) >> 8;
      uint8_t q = (val * (255 - ((sat * remainder) >> 8))) >> 8;
      uint8_t t = (val * (255 - ((sat * (255 - remainder)) >> 8))) >> 8;

      switch (region)
      {
      case 0:
        return Color(val, t, p);
      case 1:
        return Color(q, val, p);
      case 2:
        return Color(p, val, t);
      case 3:
        return Color(p, q, val);
      case 4:
        return Color(t, p, val);
      default:
        return Color(val, p, q);
      }
    }

    // Common colors
    static constexpr Color Black() { return Color(0, 0, 0); }
    static constexpr Color White() { return Color(255, 255, 255); }
    static constexpr Color Red() { return Color(255, 0, 0); }
    static constexpr Color Green() { return Color(0, 255, 0); }
    static constexpr Color Blue() { return Color(0, 0, 255); }
    static constexpr Color Yellow() { return Color(255, 255, 0); }
    static constexpr Color Cyan() { return Color(0, 255, 255); }
    static constexpr Color Magenta() { return Color(255, 0, 255); }
    static constexpr Color Orange() { return Color(255, 165, 0); }
    static constexpr Color Purple() { return Color(128, 0, 128); }
  };

} // namespace LumynLabs
