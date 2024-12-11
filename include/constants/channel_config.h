#pragma once

#include <array>
#include <string>

#include "constants/board_variant.h"

namespace Configuration {
static const std::array<std::string, CX_BOARD_LED_CHANNEL_COUNT>
    kBoardConfigLedChannelKeys = {
        CX_BOARD_LED_CHANNEL_1_KEY,
};
}