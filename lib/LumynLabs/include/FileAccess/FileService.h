#pragma once

#include <Arduino.h>
#include <FS.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <semphr.h>
#include <task.h>

#include <memory>

#include "ConfigurationParser/ConfigurationParser.h"
#include "FileAccess/BitmapReader.h"
#include "Networking/NetworkingGlobalMutexes.h"
#include "lumyn/domain/file/Files.h"

class FileService {
 public:
  struct TaskNotification {
    TaskHandle_t taskToNotify;
    uint8_t notificationIndex;
  };
  struct QueuedBitmapRequest {
    std::string_view path;
    TaskNotification notify;
    QueueHandle_t responseQueue;
  };

  struct QueuedBitmapResponse {
    BitmapReader::ReadResult readResult;
    Bitmap::BitmapInfo bmpInfo;
    // ! Make sure to call delete[] if result is Ok
    uint8_t* data;
  };

  struct QueuedConfigRequest {
    std::string_view path;
    TaskNotification notify;
    QueueHandle_t responseQueue;
  };

  struct QueuedConfigResponse {
    Configuration::ParseResult result;
    // ! Make sure to call delete if result is Valid
    Configuration::CXv3Configuration* config;
  };

  struct QueuedFileRequest {
    lumyn::internal::Files::Files* info;
    TaskNotification notify;
    QueueHandle_t responseQueue;
  };

  struct QueuedFileResponse {
    bool success;
  };

  struct QueuedFileReadRequest {
    std::string_view path;
    TaskNotification notify;
    QueueHandle_t responseQueue;
  };

  struct QueuedFileReadResponse {
    bool success;
    uint32_t fileSize;
    // ! Make sure to call delete[] if result is Ok
    uint8_t* data;
  };

  FileService(fs::FS& fs, uint8_t maxQueueElements = 20,
              TickType_t queueTimeout = pdMS_TO_TICKS(200),
              SemaphoreHandle_t fsMutex = NULL);

  void init(TickType_t maxFileWait = portMAX_DELAY);
  void start(uint32_t stackSize, UBaseType_t priority);

  // ! Must be used if performing file operations from another Task
  bool readBitmapQueued(QueuedBitmapRequest* request);
  // ! Must be used if performing file operations from another Task
  bool readConfigQueued(QueuedConfigRequest* request);
  // ! Must be used if performing file operations from another Task
  bool writeFileQueued(QueuedFileRequest* request);
  // ! Must be used if performing file operations from another Task
  bool readFileQueued(QueuedFileReadRequest* request);

  inline BitmapReader* reader() const { return _bmpReader.get(); }
  inline ConfigurationParser* parser() const { return _configParser.get(); }

 private:
  static void initImpl(void* _this);
  void task(void);

  bool sendToQueueAndWait(QueueHandle_t queue, TaskNotification* notification,
                          void* data);

  void handleBitmapRequest();
  void handleConfigRequest();
  void handleWriteFileRequest();
  void handleReadFileRequest();

  void readFile(const QueuedFileReadRequest& request,
                QueuedFileReadResponse& response);

  fs::FS& _fs;
  uint8_t _maxQueueElements;
  TickType_t _queueTimeout;
  TickType_t _maxFileWait = portMAX_DELAY;
  std::unique_ptr<BitmapReader> _bmpReader;
  // TODO: Replace with a generic JsonReader and then pass to the
  // ConfigurationParser locally
  std::unique_ptr<ConfigurationParser> _configParser;
  TaskHandle_t _task;
  QueueSetHandle_t _queueSet;

  const uint8_t kQueueCount = 3;

  SemaphoreHandle_t _fsMutex = SPI0Mutex;
  QueueHandle_t _bitmapQueue;
  QueueHandle_t _configQueue;
  QueueHandle_t _writeQueue;
  QueueHandle_t _readQueue;
};

#if CX_BOARD_FEATURES_HAS_SD_CARD
extern FileService SDFileAccessService;
#endif

extern FileService InternalFileAccessService;
extern FileService* FileAccessService;