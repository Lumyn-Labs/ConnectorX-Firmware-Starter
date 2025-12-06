/**
 * @file Module.h
 * @brief Public Module template for creating custom modules
 *
 * This is the primary interface for implementing custom modules.
 * Inherit from Module<T> where T is your module's data type.
 *
 * Copyright (c) Lumyn Labs, Inc. All rights reserved.
 * Licensed under the Lumyn Labs SDK License.
 */

#pragma once

#include "ModuleConfig.h"
#include "ModuleError.h"
#include "ModulePeripherals.h"

#include <ArduinoJson.h>
#include <cstdint>
#include <cstring>
#include <vector>

namespace LumynLabs
{

  // Forward declaration for internal bridge
  namespace internal
  {
    class ModuleBridge;
  }

  /**
   * @brief Base class for custom modules
   *
   * Inherit from this class to create custom sensor/actuator modules.
   * The template parameter T defines the data structure for your module.
   *
   * Example:
   * @code
   * struct MySensorData {
   *   float temperature;
   *   float humidity;
   * };
   *
   * class MySensor : public LumynLabs::Module<MySensorData> {
   * public:
   *   MySensor(const LumynLabs::ModuleConfig& config)
   *       : Module(config) {}
   *
   *   LumynLabs::ModuleError initModule() override {
   *     // Initialize your sensor using peripherals()
   *     auto& i2c = peripherals().getI2C();
   *     i2c.beginTransmission(0x40);
   *     // ...
   *     return LumynLabs::ModuleError::ok();
   *   }
   *
   *   LumynLabs::ModuleError readData(MySensorData* dataOut) override {
   *     // Read from your sensor
   *     dataOut->temperature = 25.0f;
   *     dataOut->humidity = 50.0f;
   *     return LumynLabs::ModuleError::ok();
   *   }
   * };
   * @endcode
   *
   * @tparam T The data structure type for this module
   */
  template <typename T>
  class Module
  {
  public:
    /**
     * @brief Construct a new Module
     * @param config Module configuration from the system
     */
    explicit Module(const ModuleConfig &config);
    virtual ~Module() = default;

    // Non-copyable, non-movable
    Module(const Module &) = delete;
    Module &operator=(const Module &) = delete;
    Module(Module &&) = delete;
    Module &operator=(Module &&) = delete;

    /**
     * @brief Initialize the module
     *
     * Called once when the module is registered. Use this to set up
     * your hardware (configure I2C addresses, initialize sensors, etc.)
     *
     * @return ModuleError::ok() on success, or an error
     */
    virtual ModuleError initModule() = 0;

    /**
     * @brief Read data from the module
     *
     * Called periodically based on pollingRateMs, or on demand.
     * Populate the dataOut structure with current sensor readings.
     *
     * @param dataOut Pointer to data structure to populate
     * @return ModuleError::ok() on success, or an error
     */
    virtual ModuleError readData(T *dataOut) = 0;

    /**
     * @brief Push data/commands to the module (optional)
     *
     * Override this to handle incoming commands or configuration updates.
     * The JSON format is module-specific.
     *
     * @param json JSON data to process
     * @return true if data was processed successfully
     */
    virtual bool pushData(ArduinoJson::JsonVariantConst json) { return false; }

    /**
     * @brief Get module ID
     * @return The unique module ID
     */
    uint16_t getId() const { return _config.id; }

    /**
     * @brief Get module configuration
     * @return Reference to the module configuration
     */
    const ModuleConfig &config() const { return _config; }

    /**
     * @brief Access hardware peripherals
     *
     * Use this to access I2C, SPI, or UART interfaces configured
     * for your board variant.
     *
     * @return Reference to peripheral accessors
     */
    ModulePeripherals &peripherals() { return *_peripherals; }

  protected:
    /**
     * @brief Internal read implementation
     *
     * Converts typed data to raw bytes for the system.
     * Do not override - implement readData() instead.
     */
    ModuleError read(std::vector<uint8_t> &dataOut);

  private:
    friend class internal::ModuleBridge;

    const ModuleConfig &_config;
    ModulePeripherals *_peripherals;
  };

  // Template implementation
  template <typename T>
  Module<T>::Module(const ModuleConfig &config) : _config(config), _peripherals(nullptr) {}

  template <typename T>
  ModuleError Module<T>::read(std::vector<uint8_t> &dataOut)
  {
    T data;
    ModuleError err = readData(&data);

    if (err.isOk())
    {
      dataOut.resize(sizeof(T));
      std::memcpy(dataOut.data(), &data, sizeof(T));
    }

    return err;
  }

} // namespace LumynLabs
