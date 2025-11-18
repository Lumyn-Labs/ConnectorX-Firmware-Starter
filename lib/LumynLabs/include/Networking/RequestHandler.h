#pragma once

#include <Arduino.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>

#include "LedService/LEDService.h"
#include "SystemService/SystemContext.h"
#include "SystemService/SystemService.h"
#if CX_BOARD_FEATURES_ENABLE_MODULES
#include "Modules/ModuleManager.h"
#endif

#include "lumyn/domain/request/Request.h"
#include "lumyn/domain/response/Response.h"
#include "lumyn/domain/transmission/Transmission.h"

namespace Request = lumyn::internal::Request;
namespace Response = lumyn::internal::Response;
// Avoid name clash with the class 'Transmission'
namespace Tx = lumyn::internal::Transmission;

class RequestHandler {
 public:
  RequestHandler();

  // Initialize internal queue and timeout; provide an outbound queue for
  // responses
  void init(uint16_t queueSize, TickType_t queueTimeout,
            QueueHandle_t outboundQueue);
  void start(uint32_t stackSize, UBaseType_t priority);

  // Enqueue a Request transmission (queue takes a ref; consumer adopts)
  void enqueue(Tx::Transmission* tx);

 private:
  // Task loop
  static void taskImpl(void* _this);
  void taskLoop();

  // Parse request transmission, build and send response
  void handleTransmission(const Tx::Transmission* tx);
  void sendResponse(const Response::ResponseHeader& header,
                    const void* responseData, size_t responseDataSize);

  // Per-request handlers - build ResponseHeader + response data, then call
  // sendResponse
  void handleHandshake(const Request::RequestHeader* reqHeader,
                       const Request::RequestHandshakeInfo* req);
  void handleStatus(const Request::RequestHeader* reqHeader);
  void handleProductSKU(const Request::RequestHeader* reqHeader);
  void handleProductSerialNumber(const Request::RequestHeader* reqHeader);
  void handleConfigHash(const Request::RequestHeader* reqHeader);
  void handleAssignedId(const Request::RequestHeader* reqHeader);
  void handleFaults(const Request::RequestHeader* reqHeader);
  void handleConfigFull(const Request::RequestHeader* reqHeader);

  // Stubs for unimplemented types
  void handleModuleStatus(const Request::RequestHeader* reqHeader,
                          const Request::RequestModuleStatusInfo* req);
  void handleModuleData(const Request::RequestHeader* reqHeader,
                        const Request::RequestModuleDataInfo* req);
  void handleLEDChannelStatus(const Request::RequestHeader* reqHeader,
                              const Request::RequestLEDChannelStatusInfo* req);
  void handleLEDZoneStatus(const Request::RequestHeader* reqHeader,
                           const Request::RequestLEDZoneStatusInfo* req);
  void handleLatestEvent(const Request::RequestHeader* reqHeader);
  void handleEventFlags(const Request::RequestHeader* reqHeader);

  LEDService& _ledService = LedService;
  SystemService& _systemService = SystemManagerService;
#if CX_BOARD_FEATURES_ENABLE_MODULES
  Modules::ModuleManager& _moduleService = ModuleService;
#endif

  // Queues and task
  QueueHandle_t _queue = nullptr;
  QueueHandle_t _outboundQueue = nullptr;  // where Responses are posted
  TaskHandle_t _task = nullptr;
  TickType_t _queueTimeout = 0;
  // FileService response queue
  QueueHandle_t _fileResQueue = nullptr;
};