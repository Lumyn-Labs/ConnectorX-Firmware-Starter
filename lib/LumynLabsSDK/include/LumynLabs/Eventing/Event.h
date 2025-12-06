/**
 * @file Event.h
 * @brief Public event structures for custom firmware
 *
 * Copyright (c) Lumyn Labs, Inc. All rights reserved.
 * Licensed under the Lumyn Labs SDK License.
 */

#pragma once

#include "EventType.h"
#include <cstdint>
#include <cstring>

namespace LumynLabs
{

  /**
   * @brief Cause of system disable
   */
  enum class DisabledCause : uint8_t
  {
    NoHeartbeat, ///< No heartbeat received
    Manual,      ///< Manually disabled
    EStop,       ///< Emergency stop
    Restart      ///< Restarting
  };

  /**
   * @brief Connection type
   */
  enum class ConnectionType : uint8_t
  {
    USB,    ///< Standard USB
    WebUSB, ///< WebUSB
    I2C,    ///< I2C bus
    CAN,    ///< CAN bus
    UART,   ///< Serial UART
  };

  /**
   * @brief Custom event data
   *
   * For user-defined events using EventType::Custom
   */
  struct CustomEventData
  {
    uint8_t type;     ///< User-defined type identifier
    uint8_t data[16]; ///< Custom payload
    uint8_t length;   ///< Actual length of data used
  };

  /**
   * @brief Pin interrupt event data
   */
  struct PinInterruptData
  {
    uint8_t pin; ///< GPIO pin that triggered
    void *param; ///< User parameter
  };

  /**
   * @brief Heartbeat event data
   */
  struct HeartBeatData
  {
    Status status;        ///< Current system status
    uint8_t enabled;      ///< Enabled flag
    uint8_t connectedUSB; ///< USB connection status
    uint8_t canOK;        ///< CAN bus status
  };

  /**
   * @brief Event data union
   *
   * Contains event-specific data. Use the appropriate field
   * based on the event type.
   */
  union EventData
  {
    DisabledCause disabledCause;
    ConnectionType connectionType;
    CustomEventData custom;
    PinInterruptData pinInterrupt;
    HeartBeatData heartBeat;
  };

  /**
   * @brief Event structure
   *
   * Represents a system event. Received via event queues.
   */
  struct Event
  {
    EventType type; ///< Event type
    EventData data; ///< Event-specific data

    /**
     * @brief Get optional extra message
     * @return Pointer to message data, or nullptr
     */
    const uint8_t *getExtraMessage() const;

    /**
     * @brief Get extra message length
     * @return Message length in bytes
     */
    uint16_t getExtraMessageLength() const;

    /**
     * @brief Check if event has extra message
     */
    bool hasExtraMessage() const;
  };

  /**
   * @brief Create a custom event
   *
   * Helper to create EventType::Custom events.
   *
   * @param customType User-defined type ID
   * @param data Custom data payload
   * @param length Data length (max 16)
   * @return Event configured as custom event
   */
  inline Event createCustomEvent(uint8_t customType, const uint8_t *data = nullptr, uint8_t length = 0)
  {
    Event evt;
    evt.type = EventType::Custom;
    evt.data.custom.type = customType;
    evt.data.custom.length = (length > 16) ? 16 : length;
    if (data && length > 0)
    {
      std::memcpy(evt.data.custom.data, data, evt.data.custom.length);
    }
    return evt;
  }

} // namespace LumynLabs
