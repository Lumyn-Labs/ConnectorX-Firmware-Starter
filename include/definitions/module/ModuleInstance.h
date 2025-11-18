#pragma once

#include <functional>
#include <memory>
#include <string>

#include "ConfigurationParser/configs/Module.h"
#include "Modules/ModuleManager.h"

struct ModuleInstance {
  std::string id;
  Modules::ModuleManager::ModuleCtor cb;
};