#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include <map>
#include <optional>

#include "IDCreator/IDCreator.h"
#include "definitions/led/AnimationSequenceInstance.h"

class AnimationSequenceManager {
 public:
  AnimationSequenceManager();

  uint16_t registerAnimationSequence(
      Animation::AnimationSequenceInstance&& instance);

  inline std::optional<const Animation::AnimationSequenceInstance*> getSequence(
      uint16_t id) const {
    if (_sequences.contains(id)) {
      return &_sequences.at(id);
    }

    return std::nullopt;
  }

 private:
  std::map<uint16_t, Animation::AnimationSequenceInstance> _sequences;
};