/**
 * @file Module.h
 * @brief Public Module template for creating custom modules.
 *
 * Inherit from Module<T> where T is your module's packed data type.
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
#include <functional>
#include <vector>

namespace LumynLabs
{

  namespace internal
  {
    class ModuleBridge;

    template <typename T, typename UserModule>
    class ModuleAdapter;
  }

  /**
   * @brief Base class for custom modules.
   *
   * @tparam T Packed data structure for this module's readings.
   *
   * @code
   * struct MySensorData {
   *   float temperature;
   *   float humidity;
   * };
   *
   * class MySensor : public LumynLabs::Module<MySensorData> {
   * public:
   *   MySensor(const LumynLabs::ModuleConfig& config) : Module(config) {}
   *
   *   LumynLabs::ModuleError initModule() override { ... }
   *   LumynLabs::ModuleError readData(MySensorData* dataOut) override { ... }
   * };
   * @endcode
   */
  template <typename T>
  class Module
  {
  public:
    explicit Module(const ModuleConfig &config);
    virtual ~Module() = default;

    Module(const Module &) = delete;
    Module &operator=(const Module &) = delete;
    Module(Module &&) = delete;
    Module &operator=(Module &&) = delete;

    /** Initialize the module hardware. Called once during registration. */
    virtual ModuleError initModule() = 0;

    /** Read current data from the module. */
    virtual ModuleError readData(T *dataOut) = 0;

    /**
     * Handle typed data received from the host (inverse of readData).
     * Override to process incoming binary payloads reinterpreted as T.
     */
    virtual bool handleReceivedData(const T &data) { return false; }

    /**
     * Handle JSON data received from the host.
     * Override to process incoming JSON commands or configuration updates.
     */
    virtual bool handleReceivedJson(ArduinoJson::JsonVariantConst json) { return false; }

    /** Push JSON data from this module to the host application. */
    bool pushJsonToHost(ArduinoJson::JsonVariantConst json);

    uint16_t getId() const { return _config.id; }
    const ModuleConfig &config() const { return _config; }
    ModulePeripherals &peripherals() { return *_peripherals; }

    /// @internal SDK registration support — do not call directly.
    void _sdk_setPushJsonFn(std::function<bool(ArduinoJson::JsonVariantConst)> fn)
    {
      _pushJsonToHostFn = std::move(fn);
    }

    /// @internal SDK registration support — do not call directly.
    void _sdk_setPeripherals(TwoWire &wire, arduino::HardwareSPI &spi, SerialUART &uart)
    {
      _peripherals = new ModulePeripherals(wire, spi, uart);
    }

  protected:
    /**
     * Internal: serializes typed data to raw bytes for the system.
     * Do not override — implement readData() instead.
     */
    ModuleError read(std::vector<uint8_t> &dataOut);

  private:
    friend class internal::ModuleBridge;

    template <typename U, typename V>
    friend class internal::ModuleAdapter;

    const ModuleConfig &_config;
    ModulePeripherals *_peripherals;
    std::function<bool(ArduinoJson::JsonVariantConst)> _pushJsonToHostFn;
  };

  // ── Template implementation ──────────────────────────────────────────

  template <typename T>
  Module<T>::Module(const ModuleConfig &config)
      : _config(config), _peripherals(nullptr), _pushJsonToHostFn(nullptr) {}

  template <typename T>
  bool Module<T>::pushJsonToHost(ArduinoJson::JsonVariantConst json)
  {
    if (_pushJsonToHostFn)
    {
      return _pushJsonToHostFn(json);
    }
    return false;
  }

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
