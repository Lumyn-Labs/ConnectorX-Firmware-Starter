#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include <functional>
#include <map>
#include <optional>

#include "Logger/Logger.h"
#include "definitions/led/AnimationInstance.h"
#include "lumyn/util/hashing/IDCreator.h"

class AnimationManager {
 public:
  AnimationManager();

  uint16_t registerAnimation(const Animation::AnimationInstance& instance);

  inline std::optional<const Animation::AnimationInstance*> getAnimation(
      uint16_t id) const {
    if (_animations.contains(id)) {
      return &_animations.at(id);
    }

    SerialLogger.error("  Animation ID %u not found. Available IDs:", id);
    for (const auto& [key, value] : _animations) {
      SerialLogger.error("    ID=%u, name='%s'", key, value.id.data());
    }

    return std::nullopt;
  }

  inline std::map<uint16_t, Animation::AnimationInstance>& getAllAnimations() {
    return _animations;
  }

 private:
  std::map<uint16_t, Animation::AnimationInstance> _animations;
};

extern AnimationManager AnimationMngr;