#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include <functional>
#include <map>
#include <optional>

#include "IDCreator/IDCreator.h"
#include "definitions/led/AnimationInstance.h"

class AnimationManager {
 public:
  AnimationManager();

  uint16_t registerAnimation(Animation::AnimationInstance&& instance);

  inline std::optional<const Animation::AnimationInstance*> getAnimation(
      uint16_t id) const {
    if (_animations.contains(id)) {
      return &_animations.at(id);
    }

    return std::nullopt;
  }

 private:
  std::map<uint16_t, Animation::AnimationInstance> _animations;
};

extern AnimationManager AnimationMngr;