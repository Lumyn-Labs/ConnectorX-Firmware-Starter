#pragma once

#include <Arduino.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <semphr.h>
#include <task.h>

#include <bitset>
#include <map>

#include "lumyn/domain/event/Event.h"

namespace Eventing = lumyn::internal::Eventing;

class EventingService {
 public:
  EventingService(TickType_t queueTimeout = pdMS_TO_TICKS(200));

  void init(uint16_t eventQueueSize = 50);

  // Should be called **after** all subscribers have been registered
  void start(uint32_t stackSize, UBaseType_t priority = 25);

  void registerSubscriber(QueueHandle_t queueHandle, uint32_t events);
  bool unregisterSubscriber(QueueHandle_t queueHandle);
  bool subscribeToEvent(QueueHandle_t queueHandle, Eventing::EventType type);
  bool unsubscribeFromEvent(QueueHandle_t queueHandle,
                            Eventing::EventType type);

  std::bitset<Eventing::kEventBitCount> getSubscriptionBitmask(
      QueueHandle_t queueHandle) const;

  void sendEvent(Eventing::Event* evt);
  void sendEventFromISR(Eventing::Event* evt,
                        BaseType_t* xHigherPriorityTaskWoken);

 private:
  static void initImpl(void* _this);
  void task(void);

  uint16_t _eventQueueSize;
  TickType_t _queueTimeout;

  std::map<QueueHandle_t, std::bitset<Eventing::kEventBitCount>> _subscriptions;
  TaskHandle_t _task;
  QueueHandle_t _eventQueue;
  SemaphoreHandle_t _subscribersMtx;
};

extern EventingService EventService;