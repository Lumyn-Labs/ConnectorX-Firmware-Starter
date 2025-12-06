/**
 * @file ModuleRegistration.h
 * @brief Public helper for registering custom modules
 *
 * Copyright (c) Lumyn Labs, Inc. All rights reserved.
 * Licensed under the Lumyn Labs SDK License.
 */

#pragma once

#include "Module.h"
#include <functional>
#include <memory>
#include <string>

// Forward declare internal types
namespace Modules
{
  class ModuleManager;
}

namespace LumynLabs
{

  /**
   * @brief Module registration helper
   *
   * Use this to register your custom module types with the system.
   *
   * Example:
   * @code
   * // In your setup() or initialization:
   * LumynLabs::registerModule<MySensorData, MySensor>("MY_SENSOR");
   * @endcode
   *
   * @tparam T Data type for the module
   * @tparam UserModule Your module class (must inherit from Module<T>)
   * @param typeIdentifier Unique string identifier for this module type
   */
  template <typename T, typename UserModule>
  void registerModule(const std::string &typeIdentifier);

  /**
   * @brief Module factory type
   *
   * Internal factory function type for creating module instances.
   */
  using ModuleFactory = std::function<void *(const ModuleConfig &)>;

  namespace internal
  {

    /**
     * @brief Get the module manager instance
     * @return Reference to the global ModuleManager
     */
    Modules::ModuleManager &getModuleManager();

    /**
     * @brief Register a module factory
     *
     * Internal function - use registerModule<T, M>() instead.
     */
    void registerModuleFactory(const std::string &typeIdentifier, ModuleFactory factory);

  } // namespace internal

  // Template implementation - defined in internal header
  template <typename T, typename UserModule>
  void registerModule(const std::string &typeIdentifier)
  {
    static_assert(std::is_base_of<Module<T>, UserModule>::value,
                  "UserModule must inherit from LumynLabs::Module<T>");

    // Factory is implemented in internal code that bridges to ModuleManager
    internal::registerModuleFactory(typeIdentifier, [](const ModuleConfig &config) -> void *
                                    { return new UserModule(config); });
  }

} // namespace LumynLabs
