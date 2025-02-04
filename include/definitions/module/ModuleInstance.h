#pragma once

#include <functional>
#include <memory>
#include <string>

#include "ConfigurationParser/configs/Sensor.h"
#include "Modules/ModuleManager.h"

struct ModuleInstance {
  std::string id;
  Modules::ModuleManager::ModuleCtor cb;
};