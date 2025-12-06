/**
 * @file ModuleError.h
 * @brief Public error types for custom modules
 *
 * Copyright (c) Lumyn Labs, Inc. All rights reserved.
 * Licensed under the Lumyn Labs SDK License.
 */

#pragma once

#include <cstdint>

namespace LumynLabs
{

  /**
   * @brief Module error types
   */
  enum class ModuleErrorType : uint8_t
  {
    None = 0,          ///< No error
    NotInitialized,    ///< Module not initialized
    InvalidConfig,     ///< Invalid configuration
    CommunicationFail, ///< Communication failure (I2C/SPI/UART)
    Timeout,           ///< Operation timed out
    InvalidData,       ///< Invalid data received/sent
    NotSupported,      ///< Operation not supported
    InternalError,     ///< Internal module error
    UserDefined = 0x80 ///< Start of user-defined error codes
  };

  /**
   * @brief Module error structure
   */
  struct ModuleError
  {
    ModuleErrorType errorType = ModuleErrorType::None;
    uint8_t errorCode = 0; ///< Optional additional error code

    ModuleError() = default;
    ModuleError(ModuleErrorType type, uint8_t code = 0)
        : errorType(type), errorCode(code) {}

    bool isOk() const { return errorType == ModuleErrorType::None; }
    explicit operator bool() const { return !isOk(); }

    static ModuleError ok() { return ModuleError(); }
    static ModuleError error(ModuleErrorType type, uint8_t code = 0)
    {
      return ModuleError(type, code);
    }
  };

} // namespace LumynLabs
