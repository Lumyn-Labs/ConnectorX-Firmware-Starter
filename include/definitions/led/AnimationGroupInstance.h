#pragma once

#include <Arduino.h>

#include <vector>

#include "LedService/Zone.h"

namespace Animation {
struct AnimationGroupInstance {
  std::string_view id;
  std::vector<Zone*> zones;
};
};  // namespace Animation