#pragma once

#include <Arduino.h>

#include "LedService/LEDService.h"
#include "SystemService/SystemService.h"
#include "lumyn/domain/transmission/Transmission.h"
#include "lumyn/domain/command/Command.h"                  // CommandHeader + subtypes
#include "lumyn/domain/command/system/SystemCommand.h"     // SystemCommand, SystemCommandData
#include "lumyn/domain/command/led/LEDCommand.h"           // LEDCommand, LEDCommandData

namespace Command = lumyn::internal::Command;
namespace Transmission = lumyn::internal::Transmission;

class CommandHandler {
 public:
  CommandHandler();

  void init(uint16_t queueSize, TickType_t queueTimeout);
  void start(uint32_t stackSize, UBaseType_t priority);

  // Queue takes a ref; consumer adopts it.
  void enqueue(Transmission::Transmission* tx);

 private:
  static void taskImpl(void* _this);
  void taskLoop();

  void handleTransmission(const Transmission::Transmission* tx);

  void handleSystemCommand(Command::System::SystemCommandType type,
                           const Command::System::SystemCommandData* payload);
  void handleLedCommand(Command::LED::LEDCommandType type,
                        const Command::LED::LEDCommandData* payload);

  LEDService& _ledService = LedService;

  QueueHandle_t _queue = nullptr;
  TaskHandle_t _task = nullptr;
  TickType_t _queueTimeout = 0;
};