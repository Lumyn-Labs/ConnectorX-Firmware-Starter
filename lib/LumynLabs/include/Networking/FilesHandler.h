#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include <memory>
#include <optional>

#include "Eventing/EventingService.h"
#include "FileAccess/FileService.h"
#include "LedService/LEDService.h"
#include "definitions/domain/file/Files.h"

class FilesHandler {
 public:
  FilesHandler(ConfigurationValidator&);

  void handle(Files::Files* file);

 private:
  std::optional<Eventing::Event> saveFile(Files::Files*);
  std::optional<Eventing::Event> saveConfig(Files::Files*);
  std::optional<Eventing::Event> setZonePixels(Files::Files*);

  FileService** _fileService = &FileAccessService;
  ConfigurationValidator& _configValidator;

  TickType_t _queueTimeout = portMAX_DELAY;
  const uint8_t kQueueCount = 10;

  QueueHandle_t _fileResQueue;
};