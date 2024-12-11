#pragma once

#include "AnimationInstance.h"
#include "ConfigurationParser/configs/Animation.h"

namespace Animation {
struct ActionInstanceConfig {
  const AnimationInstance* definition;
  const Configuration::Animation* action;
};

struct AnimationSequenceInstance {
  std::string_view id;
  std::vector<ActionInstanceConfig> configs;
};
}  // namespace Animation