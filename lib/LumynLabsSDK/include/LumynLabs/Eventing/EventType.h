/**
 * @file EventType.h
 * @brief Public event types for custom firmware
 *
 * Copyright (c) Lumyn Labs, Inc. All rights reserved.
 * Licensed under the Lumyn Labs SDK License.
 */

#pragma once

#include <cstdint>
#include <initializer_list>

#include "lumyn/domain/event/EventType.h"

namespace LumynLabs
{

  /**
   * @brief Event types that can be subscribed to
   *
   * Each type is one bit, so types can be combined into a subscription mask.
   */
  using EventType = lumyn::event::EventType;

  /**
   * @brief Combine multiple event types for subscription
   *
   * @param types Event types to combine
   * @return Combined event bitmask
   *
   * Example:
   * @code
   * uint32_t events = combineEvents({EventType::Connected, EventType::Module});
   * eventService.registerSubscriber(myQueue, events);
   * @endcode
   */
  constexpr uint32_t combineEvents(std::initializer_list<EventType> types)
  {
    uint32_t result = 0;
    for (auto t : types)
    {
      result |= static_cast<uint32_t>(t);
    }
    return result;
  }

  /**
   * @brief System status: Unknown, Booting, Active, Error or Fatal
   */
  using Status = lumyn::event::Status;

} // namespace LumynLabs
