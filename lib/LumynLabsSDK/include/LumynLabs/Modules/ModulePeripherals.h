/**
 * @file ModulePeripherals.h
 * @brief Opaque peripheral accessors for custom modules
 *
 * Provides access to I2C, SPI, and UART peripherals without exposing
 * internal board configuration details.
 *
 * Copyright (c) Lumyn Labs, Inc. All rights reserved.
 * Licensed under the Lumyn Labs SDK License.
 */

#pragma once

// Forward declarations - actual types are from Arduino/RP2040 SDK
class TwoWire;
namespace arduino
{
  class HardwareSPI;
}
class SerialUART;

namespace LumynLabs
{

  /**
   * @brief Peripheral access for custom modules
   *
   * This class provides opaque access to hardware peripherals (I2C, SPI, UART)
   * configured for the current board variant. The actual pin assignments and
   * configuration are handled internally by the SDK.
   *
   * @note Peripheral availability depends on module configuration.
   *       Check your module's connectionType to determine which peripheral to use.
   */
  class ModulePeripherals
  {
  public:
    /**
     * @brief Get the I2C (Wire) interface
     * @return Reference to the configured TwoWire instance
     *
     * Use this for modules with I2C connection type.
     * The SDK automatically configures the correct pins for your board variant.
     */
    TwoWire &getI2C();

    /**
     * @brief Get the SPI interface
     * @return Reference to the configured HardwareSPI instance
     *
     * Use this for modules with SPI connection type.
     * The SDK automatically configures the correct pins for your board variant.
     */
    arduino::HardwareSPI &getSPI();

    /**
     * @brief Get the UART interface
     * @return Reference to the configured SerialUART instance
     *
     * Use this for modules with UART connection type.
     * The SDK automatically configures the correct pins for your board variant.
     */
    SerialUART &getUART();

  private:
    // Only Module<T> can construct this
    template <typename T>
    friend class Module;

    ModulePeripherals(TwoWire &wire, arduino::HardwareSPI &spi, SerialUART &uart);

    TwoWire &_wire;
    arduino::HardwareSPI &_spi;
    SerialUART &_uart;
  };

} // namespace LumynLabs
