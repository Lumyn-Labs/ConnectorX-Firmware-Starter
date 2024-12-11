#pragma once

#include <Arduino.h>

#include <array>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "ConfigurationParser/configs/AnimationGroup.h"
#include "ConfigurationParser/configs/AnimationSequence.h"
#include "ConfigurationParser/configs/Bitmap.h"
#include "ConfigurationParser/configs/Channel.h"
#include "ConfigurationParser/configs/Network.h"
#include "ConfigurationParser/configs/Sensor.h"
#include "constants/board_variant.h"

namespace Configuration {
struct CXv3Configuration {
  uint8_t md5[16];
  std::optional<std::string> teamNumber;
  Network network;
  std::optional<std::vector<Configuration::Channel>> channels;
  std::optional<std::vector<AnimationSequence>> sequences;
  std::optional<std::vector<Bitmap>> bitmaps;
  std::optional<std::vector<AnimationGroup>> animationGroups;
  std::optional<std::vector<Sensor>> sensors;
};
}  // namespace Configuration
