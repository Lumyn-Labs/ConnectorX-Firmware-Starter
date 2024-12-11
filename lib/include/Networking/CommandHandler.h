#pragma once

#include <Arduino.h>

#include "LedService/LEDService.h"
#include "SystemService/SystemService.h"
#include "definitions/domain/command/Command.h"

class CommandHandler {
 public:
  CommandHandler();

  void handle(Command::Command* cmd);

 private:
  LEDService& _ledService = LedService;
  // SystemService& _systemService = SystemManagerService;
};