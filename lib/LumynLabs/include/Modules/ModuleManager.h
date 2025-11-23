#pragma once

#include <Arduino.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "ConfigurationParser/configs/Module.h"
#include "Eventing/EventingService.h"
#include "IModule.h"

namespace Modules {

class ModuleManager {
 public:
  struct ModuleContext {
    std::shared_ptr<IModule> module;
    uint32_t lastPolledTs;
  };

  typedef std::function<std::shared_ptr<IModule>(const Configuration::Module&)>
      ModuleCtor;

  ModuleManager(uint16_t queueSize = 25,
                TickType_t queueTimeout = pdMS_TO_TICKS(200));

  void init(const std::vector<Configuration::Module>& modules);
  void start(uint32_t stackSize, UBaseType_t priority = 4);

  void registerModuleType(const std::string& typeIdentifier, ModuleCtor ctor) {
    _factories.insert({typeIdentifier, ctor});
  }

  std::optional<ModuleContext*> getModuleById(uint16_t id) {
    if (_modules.contains(id)) {
      return &_modules.at(id);
    }
    return std::nullopt;
  }

  bool readModuleData(uint16_t moduleId, std::vector<uint8_t>& dataOut);
  bool readAndTransmitModuleData(uint16_t moduleId,
                                 Eventing::Event* evtOut = nullptr);
  std::vector<uint8_t> buildModuleDataPacket(uint16_t moduleId,
                                             const std::vector<uint8_t>& data);
  void handlePushData(uint16_t moduleId, ArduinoJson::JsonVariantConst json);

 private:
  static void initImpl(void* _this);
  void task();

  bool readModuleDataWithMutex(ModuleContext* ctx,
                               std::vector<uint8_t>& dataOut);
  void handleReadError(uint16_t moduleId, const ModuleError_t& err);
  void transmitModuleData(uint16_t moduleId, const std::vector<uint8_t>& data);

  std::optional<std::shared_ptr<IModule>> createModule(
      const Configuration::Module& config);

  std::map<std::string, ModuleCtor> _factories;
  std::map<uint16_t, ModuleContext> _modules;

  uint16_t _eventQueueSize;
  TickType_t _queueTimeout;

  TaskHandle_t _task;

  EventingService& _evtService = EventService;
};

}  // namespace Modules

// External references
extern SemaphoreHandle_t Wire1Mtx;
extern Modules::ModuleManager ModuleService;