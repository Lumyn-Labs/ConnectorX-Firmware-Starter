#pragma once

#include <Arduino.h>

#include "Modules/builtin_modules/AS7341.h"
#include "Modules/builtin_modules/AnalogInput.h"
#include "Modules/builtin_modules/DigitalInput.h"
#include "Modules/builtin_modules/VL53L1X.h"
#include "definitions/module/ModuleInstance.h"

namespace Modules::Builtin {
static ModuleInstance VL53L1X{
    .id = "VL53L1X", .cb = [](const Configuration::Sensor& cfg) {
      return std::make_shared<Modules::Builtin::VL53L1XModule>(cfg);
    }};

static ModuleInstance DigitalInput{
    .id = "DigitalInput", .cb = [](const Configuration::Sensor& cfg) {
      return std::make_shared<Modules::Builtin::DigitalInputModule>(cfg);
    }};

static ModuleInstance AnalogInput{
    .id = "AnalogInput", .cb = [](const Configuration::Sensor& cfg) {
      return std::make_shared<Modules::Builtin::AnalogInputModule>(cfg);
    }};

// Not registered in firmware until implementation can be validated
// static ModuleInstance AS7341{
//     .id = "AS7341", .cb = [](const Configuration::Sensor& cfg) {
//       return std::make_shared<Modules::Builtin::AS7341Module>(cfg);
//     }};

static std::vector<ModuleInstance> BuiltinModules = {VL53L1X, DigitalInput,
                                                     AnalogInput};
}  // namespace Modules::Builtin