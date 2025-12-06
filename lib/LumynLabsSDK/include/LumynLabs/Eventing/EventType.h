/**
 * @file EventType.h
 * @brief Public event types for custom firmware
 *
 * This header exposes only the safe, user-facing event types.
 * Internal events (OTA, CONFIG, NETWORK) are not exposed.
 *
 * Copyright (c) Lumyn Labs, Inc. All rights reserved.
 * Licensed under the Lumyn Labs SDK License.
 */

#pragma once

#include <cstdint>
#include <initializer_list>

namespace LumynLabs
{

  /**
   * @brief Event types that can be subscribed to
   *
   * Use these with EventingService to receive notifications
   * about system and module events.
   */
  enum class EventType : uint32_t
  {
    /**
     * @brief System initialization started
     */
    BeginInitialization = 0x00,

    /**
     * @brief System initialization completed
     *
     * Safe to start using services after receiving this event.
     */
    FinishInitialization = 0x01 << 0,

    /**
     * @brief System or component enabled
     */
    Enabled = 0x01 << 1,

    /**
     * @brief System or component disabled
     */
    Disabled = 0x01 << 2,

    /**
     * @brief External connection established
     */
    Connected = 0x01 << 3,

    /**
     * @brief External connection lost
     */
    Disconnected = 0x01 << 4,

    /**
     * @brief Recoverable error occurred
     */
    Error = 0x01 << 5,

    /**
     * @brief Fatal error occurred (system will restart)
     */
    FatalError = 0x01 << 6,

    /**
     * @brief Custom user-defined event
     *
     * Use this for your own application-specific events.
     */
    Custom = 0x01 << 8,

    /**
     * @brief Pin interrupt triggered
     *
     * Sent when a configured GPIO interrupt fires.
     */
    PinInterrupt = 0x01 << 9,

    /**
     * @brief Periodic heartbeat event
     *
     * Sent at regular intervals for status monitoring.
     */
    HeartBeat = 0x01 << 10,

    /**
     * @brief Module event
     *
     * Sent when a module has new data or status change.
     */
    Module = 0x01 << 12,
  };

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
   * @brief System status
   */
  enum class Status : int8_t
  {
    Unknown = -1, ///< Status not determined
    Booting = 0,  ///< System is starting up
    Active,       ///< System is running normally
    Error,        ///< System has recoverable error
    Fatal,        ///< System has fatal error
  };

} // namespace LumynLabs
