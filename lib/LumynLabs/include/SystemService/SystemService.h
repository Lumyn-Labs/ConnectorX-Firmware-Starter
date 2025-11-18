#pragma once

#include <Arduino.h>
#include <FreeRTOS.h>
#include <pico/binary_info.h>
#include <queue.h>
#include <task.h>

#include <memory>

#include "Eventing/EventingService.h"
#include "LedService/LEDService.h"
#include "Modules/ModuleManager.h"
#include "Status.h"
#include "SystemService/SystemContext.h"
#include "lumyn/domain/command/system/SystemCommand.h"
#include "lumyn/domain/event/Event.h"
#include "lumyn/domain/request/RequestType.h"

#define bi_program_name(CX_BOARD_INFO_NAME)
#define bi_program_version_string(CX_BOARD_INFO_VERSION)

#define PROGRAM_VERSION_MAJOR 3
#define PROGRAM_VERSION_MINOR 1
#define PROGRAM_VERSION_PATCH 1

class SystemService {
 public:
  SystemService(TickType_t queueTimeout = pdMS_TO_TICKS(20),
                Eventing::Status initStatus = Eventing::Status::Booting);

  bool init(bool logOutput = false);
  bool initServices();
  void start(uint32_t stackSize, UBaseType_t priority);

  void sendCommand(Command::System::SystemCommand*);

#if CX_BOARD_FEATURES_ENABLE_MODULES
  void registerModuleType(std::string typeIdentifier,
                          Modules::ModuleManager::ModuleCtor ctor);
#endif

  Eventing::Status getStatus(void) const;

  inline uint32_t getErrorFlags() const {
    return SystemContext.errorFlags.errors;
  }
  void clearErrorFlag(uint32_t bitmask);
  const Configuration::CXv3Configuration& config(void) const {
    return *_config;
  }

  inline const std::optional<const char*> getAssignedId() const {
    if (SystemContext.settingsValid) {
      return SystemContext.assignedId.c_str();
    }

    return std::nullopt;
  }

  inline const uint64_t getBoardId() const {
    pico_unique_board_id_t brd;

    pico_get_unique_board_id(&brd);
    LongLongBytes bytes;
    memcpy(bytes.bytes, brd.id, sizeof(bytes.bytes));

    return bytes.val;
  }

 private:
  union LongLongBytes {
    uint8_t bytes[8];
    uint64_t val;
  };

  static void taskImpl(void* _this);
  void task(void);

  void initSpi(void);
  void initI2C(void);
  void initUart(void);
  void initFs(void);
  void initPeripherals(void);
  bool parseConfig(void);

  void handleCommand(Command::System::SystemCommand*);
  void handleEvent(Eventing::Event*);
  void setStatusLed(void);
  void rebootDevice(Command::System::RestartDeviceData data);

  LEDService& _ledService = LedService;
  EventingService& _eventService = EventService;
  QueueHandle_t _eventQueue;
  QueueHandle_t _cmdQueue;
  QueueSetHandle_t _queueSet;
  TickType_t _queueTimeout;
  TaskHandle_t _task;
  std::unique_ptr<Configuration::CXv3Configuration> _config;
};

extern SystemService SystemManagerService;
extern Adafruit_USBD_CDC& USBSer1;