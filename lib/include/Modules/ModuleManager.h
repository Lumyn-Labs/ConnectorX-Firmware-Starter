#pragma once

#include <Arduino.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include "ConfigurationParser/configs/Sensor.h"
#include "Eventing/EventingService.h"
#include "IModule.h"

namespace Modules {
class ModuleManager {
 public:
  ModuleManager(uint16_t queueSize = 25,
                TickType_t queueTimeout = pdMS_TO_TICKS(200));

  void init(const std::vector<Configuration::Sensor>& sensors);
  void start(uint32_t stackSize, UBaseType_t priority = 4);

  typedef std::function<std::shared_ptr<IModule>(const Configuration::Sensor&)>
      ModuleCtor;
  void registerModuleType(std::string typeIdentifier, ModuleCtor ctor) {
    _factories.insert({typeIdentifier, ctor});
  }

 private:
  struct ModuleContext {
    std::shared_ptr<IModule> module;
    uint32_t lastPolledTs;
  };

  static void initImpl(void* _this);
  void task(void);
  std::optional<std::shared_ptr<Modules::IModule>> createModule(
      const Configuration::Sensor&);

  std::map<std::string, ModuleCtor> _factories;
  std::map<uint16_t, ModuleContext> _modules;

  uint16_t _eventQueueSize;
  TickType_t _queueTimeout;

  TaskHandle_t _task;
  QueueHandle_t _eventQueue;

  EventingService& _evtService = EventService;
};
};  // namespace Modules

extern SemaphoreHandle_t Wire1Mtx;