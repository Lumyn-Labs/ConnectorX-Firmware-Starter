#pragma once

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <FreeRTOS.h>
#include <Wire.h>
#include <queue.h>
#include <semphr.h>
#include <task.h>

#include <memory>

#include "Eventing/EventingService.h"
#include "FileAccess/FileService.h"
#include "constants/board_variant.h"

class ScreenService {
 public:
  ScreenService(TickType_t queueTimeout = pdMS_TO_TICKS(200));

  void init(uint16_t eventQueueSize = 30);
  void start(uint32_t stackSize, UBaseType_t priority = 25);

 private:
  void flushToScreen(void);

  // TODO: helpers for drawing info based on events + status

  void showSplash(void);
  void handleEvent(Eventing::Event& event);

  static void taskImpl(void* _this);
  void task(void);

  std::optional<uint8_t*> readBitmapData(
      const std::string&, FileService::QueuedBitmapResponse* response);
  void drawBitmap(uint8_t* data, FileService::QueuedBitmapResponse* response);

  std::unique_ptr<Adafruit_SSD1306> _screen;
  FileService& _fileService = InternalFileAccessService;
  EventingService& _evtService = EventService;
  TickType_t _queueTimeout;
  QueueHandle_t _bmpRecQueue;
  QueueHandle_t _evtQueue;
  TaskHandle_t _task;

  const uint16_t kWidth = CX_BOARD_SCREEN_WIDTH_PX;
  const uint16_t kHeight = CX_BOARD_SCREEN_HEIGHT_PX;
};

extern SemaphoreHandle_t Wire1Mtx;