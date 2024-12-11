#pragma once

#include <Arduino.h>

#include "LEDCommandType.h"

namespace Command {
namespace LED {
struct __attribute__((packed)) AnimationColor {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

struct __attribute__((packed)) SetAnimationData {
  uint16_t zoneId;
  uint16_t animationId;
  uint16_t delay;
  AnimationColor color;
  uint8_t reversed : 1;
  uint8_t oneShot : 1;
};

struct __attribute__((packed)) SetAnimationGroupData {
  uint16_t groupId;
  uint16_t animationId;
  uint16_t delay;
  AnimationColor color;
  uint8_t reversed : 1;
  uint8_t oneShot : 1;
};

struct __attribute__((packed)) SetColorData {
  uint16_t zoneId;
  AnimationColor color;
};

struct __attribute__((packed)) SetColorGroupData {
  uint16_t groupId;
  AnimationColor color;
};

struct __attribute__((packed)) SetAnimationSequenceData {
  uint16_t zoneId;
  uint16_t sequenceId;
};

struct __attribute__((packed)) SetAnimationSequenceGroupData {
  uint16_t groupId;
  uint16_t sequenceId;
};

struct __attribute__((packed)) SetBitmapData {
  uint16_t zoneId;
  uint16_t bitmapId;
  AnimationColor color;
  uint8_t setColor : 1;
  uint8_t oneShot : 1;
};

struct __attribute__((packed)) SetBitmapGroupData {
  uint16_t groupId;
  uint16_t bitmapId;
  AnimationColor color;
  uint8_t setColor : 1;
  uint8_t oneShot : 1;
};

enum class MatrixTextScrollDirection : uint8_t { LEFT = 0, RIGHT };

struct __attribute__((packed)) SetMatrixTextData {
  uint16_t zoneId;
  uint8_t oneShot;
  AnimationColor color;
  MatrixTextScrollDirection dir;
  char text[24];
  uint8_t length;
  uint16_t delay;
};

struct __attribute__((packed)) SetMatrixTextGroupData {
  uint16_t groupId;
  uint8_t oneShot;
  AnimationColor color;
  MatrixTextScrollDirection dir;
  char text[24];
  uint8_t length;
  uint16_t delay;
};

struct __attribute__((packed)) LEDCommand {
  LEDCommandType type;
  union {
    SetAnimationData setAnimation;
    SetAnimationGroupData setAnimationGroup;
    SetColorData setColor;
    SetColorGroupData setColorGroup;
    SetAnimationSequenceData setAnimationSequence;
    SetAnimationSequenceGroupData setAnimationSequenceGroup;
    SetBitmapData setBitmap;
    SetBitmapGroupData setBitmapGroup;
    SetMatrixTextData setMatrixText;
    SetMatrixTextGroupData setMatrixTextGroup;
  };
};

constexpr auto sz = sizeof(LEDCommand);
}  // namespace LED
}  // namespace Command