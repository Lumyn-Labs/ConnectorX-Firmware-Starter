/**
 * @file Event.h
 * @brief Public event structures for custom firmware
 *
 * Copyright (c) Lumyn Labs, Inc. All rights reserved.
 * Licensed under the Lumyn Labs SDK License.
 */

#pragma once

#include <cstdint>
#include <cstring>

#include "EventType.h"
#include "lumyn/domain/event/Event.h"

namespace LumynLabs
{

  /** @brief Cause of system disable */
  using DisabledCause = lumyn::event::DisabledCause;

  /** @brief Connection type */
  using ConnectionType = lumyn::event::ConnectionType;

  /** @brief Custom event data: a type, up to 16 bytes, and their length */
  using CustomEventData = lumyn::event::CustomInfo;

  /** @brief Pin interrupt event data: the pin and a user parameter */
  using PinInterruptData = lumyn::event::PinInterruptInfo;

  /** @brief Heartbeat event data */
  using HeartBeatData = lumyn::event::HeartBeatInfo;

  /** @brief Event data union; the member to read depends on the event type */
  using EventData = lumyn::event::EventData;

  /**
   * @brief Event structure
   *
   * The type is `evt.header.type` and the data is `evt.header.data`. An event
   * may carry extra text: see hasExtraMessage() and getExtraMessage().
   */
  using Event = lumyn::event::Event;

  /**
   * @brief Create a custom event
   *
   * @param customType User-defined type identifier
   * @param data Optional payload, at most 16 bytes; longer data is cut
   * @param length Payload length
   * @return The event, ready to publish
   */
  inline Event createCustomEvent(uint8_t customType, const uint8_t *data = nullptr, uint8_t length = 0)
  {
    Event evt;
    evt.header.type = EventType::Custom;
    auto &custom = evt.header.data.custom;
    custom.type = customType;
    custom.length = (length > sizeof(custom.data)) ? sizeof(custom.data) : length;
    if (data && length > 0)
    {
      std::memcpy(custom.data, data, custom.length);
    }
    return evt;
  }

} // namespace LumynLabs
