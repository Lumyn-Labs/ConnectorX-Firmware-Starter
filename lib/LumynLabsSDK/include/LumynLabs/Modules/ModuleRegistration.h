/**
 * @file ModuleRegistration.h
 * @brief Public helper for registering custom modules
 *
 * Copyright (c) Lumyn Labs, Inc. All rights reserved.
 * Licensed under the Lumyn Labs SDK License.
 */

#pragma once

#include <cstring>
#include <functional>
#include <memory>
#include <string>

#include "Module.h"

namespace LumynLabs {

namespace internal {

/**
 * @brief Type-erased callback table for a user module.
 *
 * Populated by the registerModule<T,UserModule> template where concrete
 * types are known, and consumed by the .a library which wraps the
 * callbacks in a proper IModule adapter.
 */
struct SdkModuleOps {
  /// Create the user module. Returns an owning void* to a Module<T> subclass.
  std::function<void*(const ModuleConfig&)> create;

  /// Call initModule() on the user module.
  std::function<ModuleError(void*)> init;

  /// Call readData() and serialize to bytes.
  std::function<ModuleError(void*, std::vector<uint8_t>&)> read;

  /// Call handleReceivedJson().
  std::function<bool(void*, ArduinoJson::JsonVariantConst)> handleJson;

  /// Call handleReceivedData() after deserialising from raw bytes.
  std::function<bool(void*, const uint8_t*, size_t)> handleBinary;

  /// Inject the push-to-host callback into the user module.
  std::function<void(void*, std::function<bool(ArduinoJson::JsonVariantConst)>)>
      setPushJsonFn;

  /// Inject peripheral references into the user module.
  std::function<void(void*, TwoWire&, arduino::HardwareSPI&, SerialUART&)>
      setPeripherals;

  /// Destroy the user module.
  std::function<void(void*)> destroy;
};

/// Register a module type using a type-erased ops table.  Implemented in .a.
void registerModuleOps(const std::string& typeIdentifier, SdkModuleOps ops);

}  // namespace internal

/**
 * @brief Register a custom module type with the system.
 *
 * @code
 * LumynLabs::registerModule<MySensorData, MySensor>("MY_SENSOR");
 * @endcode
 *
 * @tparam T       Packed data structure for this module's readings.
 * @tparam UserModule  Your module class (must inherit from Module<T>).
 * @param typeIdentifier  Unique string identifier matching the device config.
 */
template <typename T, typename UserModule>
void registerModule(const std::string& typeIdentifier) {
  static_assert(std::is_base_of<Module<T>, UserModule>::value,
                "UserModule must inherit from LumynLabs::Module<T>");

  internal::SdkModuleOps ops;

  ops.create = [](const ModuleConfig& config) -> void* {
    return static_cast<void*>(new UserModule(config));
  };

  ops.init = [](void* p) -> ModuleError {
    return static_cast<UserModule*>(p)->initModule();
  };

  ops.read = [](void* p, std::vector<uint8_t>& out) -> ModuleError {
    T data;
    ModuleError err = static_cast<UserModule*>(p)->readData(&data);
    if (err.isOk()) {
      out.resize(sizeof(T));
      std::memcpy(out.data(), &data, sizeof(T));
    }
    return err;
  };

  ops.handleJson = [](void* p, ArduinoJson::JsonVariantConst json) -> bool {
    return static_cast<UserModule*>(p)->handleReceivedJson(json);
  };

  ops.handleBinary = [](void* p, const uint8_t* data, size_t len) -> bool {
    if (len != sizeof(T)) return false;
    T typed;
    std::memcpy(&typed, data, sizeof(T));
    return static_cast<UserModule*>(p)->handleReceivedData(typed);
  };

  ops.setPushJsonFn =
      [](void* p, std::function<bool(ArduinoJson::JsonVariantConst)> fn) {
        static_cast<Module<T>*>(p)->_sdk_setPushJsonFn(std::move(fn));
      };

  ops.setPeripherals = [](void* p, TwoWire& wire, arduino::HardwareSPI& spi,
                          SerialUART& uart) {
    static_cast<Module<T>*>(p)->_sdk_setPeripherals(wire, spi, uart);
  };

  ops.destroy = [](void* p) { delete static_cast<UserModule*>(p); };

  internal::registerModuleOps(typeIdentifier, std::move(ops));
}

}  // namespace LumynLabs
