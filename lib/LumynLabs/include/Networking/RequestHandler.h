#pragma once

#include <Arduino.h>
#include <FreeRTOS.h>
#include <queue.h>

#include "LedService/LEDService.h"
#include "SystemService/SystemService.h"
#include "Modules/ModuleManager.h"
#include "definitions/domain/request/Request.h"
#include "definitions/domain/response/Response.h"

class RequestHandler {
 public:
  RequestHandler();

  Response::Response handle(Request::Request*);
  /**
   * @brief Synchronously handle the request and then enqueue the Response as a
   * Transmission
   *
   */
  void handleAsync(Request::Request*, QueueHandle_t,
                   TickType_t timeout = portMAX_DELAY);

 private:
  LEDService& _ledService = LedService;
  SystemService& _systemService = SystemManagerService;
#if CX_BOARD_FEATURES_ENABLE_DEVICES
  Modules::ModuleManager& _moduleService = ModuleService;
#endif
};