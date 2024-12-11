#pragma once

#include <Arduino.h>

#include <functional>
#include <memory>
#include <vector>

#include "CommandHandler.h"
#include "ConfigurationParser/ConfigurationValidator.h"
#include "ConfigurationParser/configs/Network.h"
#include "Eventing/EventingService.h"
#include "FileAccess/FileService.h"
#include "Networking/FilesHandler.h"
#include "NetworkingGlobalMutexes.h"
#include "RequestHandler.h"
#include "adapters/INetworkingAdapter.h"
#include "definitions/domain/event/Event.h"
#include "definitions/domain/file/Files.h"
#include "definitions/domain/module/ModuleData.h"

class NetworkingService {
 public:
  NetworkingService(ConfigurationValidator&, CommandHandler&, RequestHandler&,
                    TickType_t queueTimeout);

  void init(const Configuration::Network* config, uint16_t queueSize);
  void start(uint32_t stackSize, UBaseType_t priority);

  void transmitEvent(Eventing::Event* evt);
  void transmitResponse(Response::Response* response);
  // Only works for FileTransfer types
  void transmitFile(Files::Files* file);
  void transmitModuleData(ModuleData::ModuleData*);
  void transmit(Transmission::Transmission*);

 private:
  static void initImpl(void* _this);
  void task(void);

  void handleTransmissionCb(Transmission::Transmission*);
  void handleTransmission(Transmission::Transmission*);

  const Configuration::Network* _config = nullptr;
  EventingService& _evtService = EventService;
  FileService** _fileService = &FileAccessService;
  ConfigurationValidator& _configValidator;
  CommandHandler& _cmdHandler;
  RequestHandler& _requestHandler;
  std::unique_ptr<FilesHandler> _filesHandler;

  uint16_t _queueSize;
  TickType_t _queueTimeout;

  TaskHandle_t _task;
  QueueHandle_t _outboundQueue;
  QueueHandle_t _inboundQueue;
  QueueHandle_t _evtQueue;
  std::vector<std::shared_ptr<Networking::INetworkingAdapter>> _adapters;
};

extern NetworkingService NetworkService;