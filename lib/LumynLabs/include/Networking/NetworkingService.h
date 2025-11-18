#pragma once

#include <Arduino.h>
#include <FreeRTOS.h>
#include <queue.h>

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
#include "lumyn/domain/event/Event.h"
#include "lumyn/domain/file/Files.h"
#include "lumyn/domain/module/ModuleData.h"
#include "lumyn/domain/response/Response.h"
#include "lumyn/domain/transmission/Transmission.h"

namespace Eventing = lumyn::internal::Eventing;
namespace Files = lumyn::internal::Files;
namespace ModuleData = lumyn::internal::ModuleData;
namespace Transmission = lumyn::internal::Transmission;
namespace Response = lumyn::internal::Response;

class NetworkingService {
 public:
  NetworkingService(ConfigurationValidator&, CommandHandler&, RequestHandler&,
                    TickType_t queueTimeout);

  void init(const Configuration::Network* config, uint16_t queueSize);
  void start(uint32_t stackSize, UBaseType_t priority);

  void transmitEvent(Eventing::Event* evt);
  void transmitFile(Files::Files* file);
  void transmit(Transmission::Transmission*);

 private:
  static void txImpl(void* _this);
  static void rxImpl(void* _this);
  void txTask(void);
  void rxTask(void);

  void handleTransmissionCb(Transmission::Transmission*);
  void handleTransmission(const Transmission::Transmission*);

  const Configuration::Network* _config = nullptr;
  EventingService& _evtService = EventService;
  ConfigurationValidator& _configValidator;
  CommandHandler& _cmdHandler;
  RequestHandler& _requestHandler;
  std::unique_ptr<FilesHandler> _filesHandler;

  uint16_t _queueSize = 0;
  TickType_t _queueTimeout;

  TaskHandle_t _txTask = nullptr;
  TaskHandle_t _rxTask = nullptr;

  // Queues
  QueueHandle_t _inboundQueue =
      nullptr;  // Transmission* - incoming from adapters
  QueueHandle_t _outboundQueue =
      nullptr;  // Transmission* - outgoing to adapters (responses go here too)
  QueueHandle_t _evtQueue = nullptr;  // Event (value copy)

  // Queue set for TX task (simultaneous wait on multiple queues)
  QueueSetHandle_t _txQueueSet = nullptr;

  std::vector<std::shared_ptr<Networking::INetworkingAdapter>> _adapters;
};

extern NetworkingService NetworkService;