#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>

#include <memory>
#include <optional>

#include "ConfigurationParser/ConfigurationValidator.h"
#include "Eventing/EventingService.h"
#include "FileAccess/FileService.h"
#include "LedService/LEDService.h"
#include "Logger/Logger.h"
#include "lumyn/Constants.h"
#include "lumyn/domain/file/Files.h"
#include "lumyn/domain/transmission/Transmission.h"

namespace Files = lumyn::internal::Files;
namespace Transmission = lumyn::internal::Transmission;

class FilesHandler {
 public:
  FilesHandler(ConfigurationValidator&);

  // Async API: init queue/task and enqueue incoming file transmissions
  void init(uint16_t queueSize, TickType_t queueTimeout);
  void start(uint32_t stackSize, UBaseType_t priority);
  void enqueue(Transmission::Transmission* tx);

 private:
  static void taskImpl(void* _this);
  void taskLoop();

  // Parse transmission into a zero-copy Files view and handle it
  void handleTransmission(const Transmission::Transmission* tx);

  // Per-type handlers (operate on zero-copy Files view)
  std::optional<Eventing::Event> saveFile(Files::Files* f);
  std::optional<Eventing::Event> saveConfig(Files::Files* f);
  std::optional<Eventing::Event> setZonePixels(Files::Files* f);

  FileService** _fileService = &FileAccessService;
  ConfigurationValidator& _configValidator;

  TickType_t _queueTimeout = portMAX_DELAY;
  const uint8_t kQueueCount = 10;

  // Inbound queue of Transmission*
  QueueHandle_t _queue = nullptr;
  TaskHandle_t _task = nullptr;

  // FileService response queue
  QueueHandle_t _fileResQueue = nullptr;
};