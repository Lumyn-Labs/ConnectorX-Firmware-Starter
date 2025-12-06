/**
 * @file ModuleConfig.h
 * @brief Module configuration types for custom modules
 *
 * Copyright (c) Lumyn Labs, Inc. All rights reserved.
 * Licensed under the Lumyn Labs SDK License.
 */

#pragma once

#include <cstdint>

namespace LumynLabs
{

  /**
   * @brief Module connection type
   *
   * Specifies how the module connects to the board.
   * This determines which peripheral interface to use.
   */
  enum class ModuleConnectionType : uint8_t
  {
    I2C = 0, ///< I2C/Wire connection
    SPI,     ///< SPI connection
    UART,    ///< Serial UART connection
    DIO,     ///< Digital I/O connection
    AIO      ///< Analog I/O connection
  };

  /**
   * @brief Module configuration
   *
   * Configuration passed to modules during initialization.
   * Created by the system based on device configuration.
   */
  struct ModuleConfig
  {
    uint16_t id;                         ///< Unique module ID
    uint8_t type;                        ///< Module type identifier
    uint16_t pollingRateMs;              ///< Polling rate in milliseconds (0 = event-driven)
    ModuleConnectionType connectionType; ///< Connection interface type

    /**
     * @brief Custom configuration data
     *
     * Optional custom configuration blob. Format is module-specific.
     * Maximum size is 32 bytes.
     */
    const uint8_t *customConfig;
    uint8_t customConfigSize;
  };

} // namespace LumynLabs
