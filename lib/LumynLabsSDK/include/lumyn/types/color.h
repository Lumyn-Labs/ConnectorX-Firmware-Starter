/**
 * @file color.h
 * @brief C-compatible color type definitions
 * 
 * This header defines color types that work in both C and C++.
 */
#pragma once

#include <stdint.h>

#include "lumyn/connectorx_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief RGB color representation
 */
typedef connectorx_rgb_t lumyn_color_t;

#ifdef __cplusplus
} // extern "C"

// C++ convenience functions for colors
namespace lumyn::colors {
  inline constexpr lumyn_color_t Black() { return {0, 0, 0}; }
  inline constexpr lumyn_color_t White() { return {255, 255, 255}; }
  inline constexpr lumyn_color_t Red() { return {255, 0, 0}; }
  inline constexpr lumyn_color_t Green() { return {0, 255, 0}; }
  inline constexpr lumyn_color_t Blue() { return {0, 0, 255}; }
  inline constexpr lumyn_color_t Orange() { return {255, 165, 0}; }
  inline constexpr lumyn_color_t Yellow() { return {255, 255, 0}; }
  inline constexpr lumyn_color_t Purple() { return {128, 0, 128}; }
  inline constexpr lumyn_color_t Cyan() { return {0, 255, 255}; }
  inline constexpr lumyn_color_t Magenta() { return {255, 0, 255}; }
}

#endif // __cplusplus

