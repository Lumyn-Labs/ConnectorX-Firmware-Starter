#pragma once

#include <Arduino.h>

#include <string>
#include <vector>

namespace Configuration {
struct AnimationGroup {
  std::string id;
  std::vector<std::string> zoneIds;
};
}  // namespace Configuration