#pragma once

#include <Arduino.h>

#include <map>
#include <optional>

#include "IDCreator/IDCreator.h"
#include "definitions/led/AnimationGroupInstance.h"

class AnimationGroupManager {
 public:
  AnimationGroupManager();

  uint16_t registerAnimationGroup(Animation::AnimationGroupInstance&& instance);

  inline std::optional<const Animation::AnimationGroupInstance*> getGroup(
      uint16_t id) const {
    if (_groups.contains(id)) {
      return &_groups.at(id);
    }

    return std::nullopt;
  }

 private:
  std::map<uint16_t, Animation::AnimationGroupInstance> _groups;
};