#pragma once

#include <Arduino.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>

#include "constants/board_variant.h"
#include "lumyn/domain/module/ModuleData.h"
#include "lumyn/domain/transmission/Transmission.h"

#if CX_BOARD_FEATURES_ENABLE_MODULES
#include "Modules/ModuleManager.h"
#endif

namespace ModuleData = lumyn::internal::ModuleData;
namespace Tx = lumyn::internal::Transmission;

class ModuleHandler {
 public:
  ModuleHandler();

  void init(uint16_t queueSize, TickType_t queueTimeout);
  void start(uint32_t stackSize, UBaseType_t priority);

  // Enqueue a ModuleData transmission (queue takes a ref; consumer adopts)
  void enqueue(Tx::Transmission* tx);

 private:
  static void taskImpl(void* _this);
  void taskLoop();

  void handleTransmission(const Tx::Transmission* tx);

  // Handler for PushData type
  void handlePushData(uint16_t moduleId, const uint8_t* jsonData,
                      size_t jsonLen);

#if CX_BOARD_FEATURES_ENABLE_MODULES
  Modules::ModuleManager& _moduleService = ModuleService;
#endif

  QueueHandle_t _queue = nullptr;
  TaskHandle_t _task = nullptr;
  TickType_t _queueTimeout = 0;
};