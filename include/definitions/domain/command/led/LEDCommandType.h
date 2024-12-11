#pragma once

#include <Arduino.h>

namespace Command {
namespace LED {
enum class LEDCommandType : uint8_t {
  SetAnimation,
  SetAnimationGroup,
  SetColor,
  SetColorGroup,
  SetAnimationSequence,
  SetAnimationSequenceGroup,
  SetBitmap,
  SetBitmapGroup,
  SetMatrixText,
  SetMatrixTextGroup
};
}
}  // namespace Command