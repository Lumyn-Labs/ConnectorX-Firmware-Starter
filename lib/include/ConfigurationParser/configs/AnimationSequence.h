#pragma once

#include <Arduino.h>

#include <string_view>
#include <vector>

#include "ConfigurationParser/configs/Animation.h"

namespace Configuration {
struct AnimationSequence {
  std::string id;
  std::vector<Animation> steps;
};
}  // namespace Configuration