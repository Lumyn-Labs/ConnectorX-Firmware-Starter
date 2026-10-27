#pragma once

#include <algorithm>
#include <cinttypes>
#include <stdexcept>
#include <string>
#include <string_view>

#include "lumyn/types/led_command.h"
#include "lumyn/types/color.h"
#include "lumyn/internal_names.h"

namespace lumyn::led {
// Color class extends the basic C lumyn_color_t with additional methods
struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  
  // Conversion to/from C type
  constexpr operator lumyn_color_t() const { return {r, g, b}; }
  constexpr Color(const lumyn_color_t& c) : r(c.r), g(c.g), b(c.b) {}

  constexpr Color() : r(0), g(0), b(0) {}
  constexpr Color(uint8_t red, uint8_t green, uint8_t blue)
      : r(red), g(green), b(blue) {}

  // Equality operators
  constexpr bool operator==(const Color &other) const {
    return r == other.r && g == other.g && b == other.b;
  }

  constexpr bool operator!=(const Color &other) const {
    return !(*this == other);
  }

  // Convert RGB to HSV
  void toHSV(uint8_t &hue, uint8_t &sat, uint8_t &val) const {
    uint8_t maxVal = (r > g) ? ((r > b) ? r : b) : ((g > b) ? g : b);
    uint8_t minVal = (r < g) ? ((r < b) ? r : b) : ((g < b) ? g : b);
    uint8_t delta = maxVal - minVal;

    val = maxVal;

    if (delta == 0) {
      hue = 0;
      sat = 0;
    } else {
      sat = (255 * delta) / maxVal;

      // 43 steps of hue to each sixth of the wheel, as FromHSV. A negative
      // hue wraps around to the top.
      int h;
      if (maxVal == r) {
        h = 43 * (g - b) / delta;
      } else if (maxVal == g) {
        h = 85 + 43 * (b - r) / delta;
      } else {
        h = 171 + 43 * (r - g) / delta;
      }
      hue = static_cast<uint8_t>((h + 256) % 256);
    }
  }

  // Blend two colors together
  constexpr Color blend(const Color &other, uint8_t amount) const {
    // amount: 0 = all this, 255 = all other
    return Color(r + (((int16_t)other.r - r) * amount) / 255,
                 g + (((int16_t)other.g - g) * amount) / 255,
                 b + (((int16_t)other.b - b) * amount) / 255);
  }

  // Create color from HSV values
  static constexpr Color FromHSV(int h, int s, int v) {
    int chroma = (s * v) >> 8;
    int region = (h / 43) % 6;  // 43 = 256/6 for 0-255 hue range
    int remainder = (h % 43) * 6;
    int m = v - chroma;
    int X = (chroma * remainder) >> 8;

    switch (region) {
      case 0:
        return Color(v, X + m, m);
      case 1:
        return Color(v - X, v, m);
      case 2:
        return Color(m, v, X + m);
      case 3:
        return Color(m, v - X, v);
      case 4:
        return Color(X + m, m, v);
      default:
        return Color(v, m, v - X);
    }
  }

  // The custom-firmware SDK's name for FromHSV.
  static constexpr Color fromHSV(uint8_t hue, uint8_t sat, uint8_t val) {
    return FromHSV(hue, sat, val);
  }

  static Color fromCommand(
      const lumyn::led::AnimationColor &color) {
    return Color(color.r, color.g, color.b);
  }

  static constexpr Color Black() { return Color(0, 0, 0); }
  static constexpr Color White() { return Color(255, 255, 255); }
  static constexpr Color Red() { return Color(255, 0, 0); }
  static constexpr Color Green() { return Color(0, 255, 0); }
  static constexpr Color Blue() { return Color(0, 0, 255); }
  static constexpr Color Orange() { return Color(255, 165, 0); }
  static constexpr Color Yellow() { return Color(255, 255, 0); }
  static constexpr Color Purple() { return Color(128, 0, 128); }
  static constexpr Color Cyan() { return Color(0, 255, 255); }
  static constexpr Color Magenta() { return Color(255, 0, 255); }

  Color &operator=(const Color &other) = default;
  Color &operator=(uint32_t rgb) {
    r = (rgb >> 16) & 0xFF;
    g = (rgb >> 8) & 0xFF;
    b = rgb & 0xFF;
    return *this;
  }

  operator uint32_t() const {
    return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
  }

  void fadeLightBy(uint8_t fadeBy) {
    r = (r * (256 - fadeBy)) >> 8;
    g = (g * (256 - fadeBy)) >> 8;
    b = (b * (256 - fadeBy)) >> 8;
  }

  /**
   * @brief Scale each color component by an 8-bit value (0-255)
   *
   * @param scale The scaling factor (0-255)
   * @return Color The scaled color
   */
  Color scale8(uint8_t scale) const {
    return Color((r * scale) >> 8, (g * scale) >> 8, (b * scale) >> 8);
  }

  Color &operator=(int value) {
    if (value == 0) {
      r = g = b = 0;
    }
    return *this;
  }
};
}  // namespace lumyn::led

namespace lumyn::internal::domain {
LUMYN_INTERNAL_NAME(Color, lumyn::led::Color, "lumyn::internal::domain::Color")
}  // namespace lumyn::internal::domain
