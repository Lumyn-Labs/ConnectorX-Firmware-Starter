/**
 * @file SystemService.h
 * @brief Public System Service API wrapper
 *
 * Provides access to the SystemManagerService singleton for
 * initializing and managing the ConnectorX system.
 *
 * Copyright (c) Lumyn Labs, Inc. All rights reserved.
 * Licensed under the Lumyn Labs SDK License.
 */

#pragma once

#include <cstdint>

namespace LumynLabs
{

  /**
   * @brief System Service public interface
   *
   * Provides initialization and lifecycle management for ConnectorX.
   * Access via the global SystemManagerService instance.
   *
   * Example:
   * @code
   * void setup() {
   *   LumynLabs::System::init();
   *   // Register animations/modules here
   *   LumynLabs::System::initServices();
   * }
   * @endcode
   */
  namespace System
  {
    /**
     * @brief Initialize the system
     *
     * Must be called first in setup(). Initializes hardware,
     * networking, and core services.
     *
     * @param logOutput Enable serial logging output
     * @return true on success
     */
    bool init(bool logOutput = false);

    /**
     * @brief Complete service initialization
     *
     * Call after registering all custom animations/modules.
     * Starts all background services.
     *
     * @return true on success
     */
    bool initServices();

    /**
     * @brief Get current error flags
     * @return Bitmask of active errors
     */
    uint32_t getErrorFlags();

    /**
     * @brief Clear specific error flag
     * @param bitmask Error flag(s) to clear
     */
    void clearErrorFlag(uint32_t bitmask);

    /**
     * @brief Get unique board ID
     * @return 64-bit unique identifier
     */
    uint64_t getBoardId();
  } // namespace System

} // namespace LumynLabs
