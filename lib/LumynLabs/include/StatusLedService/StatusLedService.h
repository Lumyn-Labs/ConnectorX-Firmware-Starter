#pragma once

#include <Arduino.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>

#include "definitions/domain/command/led/LEDCommand.h"
#include "definitions/led/AnimationInstance.h"

class StatusLedService {
 public:
  StatusLedService(uint8_t maxQueueElements = 50,
                   TickType_t queueTimeout = pdMS_TO_TICKS(20));

  void init(void);
  void start(uint32_t stackSize, UBaseType_t priority);

  void setAnimation(Command::LED::SetAnimationData* animation);

 private:
  static void initImpl(void* _this);
  void task(void);
  void handleAnimation(Command::LED::SetAnimationData* animation);
  bool incrementState(void);

  inline bool shouldUpdateState() const {
    // Only update if enough delay has passed and pattern can be run again
    return millis() - _lastUpdateMs >= _curDelay;
  }

  inline void reset() {
    _curState = 0;
    _lastUpdateMs = millis();
  }

  uint8_t _maxQueueElements;
  TickType_t _queueTimeout;
  TaskHandle_t _task;
  QueueHandle_t _commandQueue;

  uint16_t _curState;
  Command::LED::AnimationColor _curColor;
  uint32_t _lastUpdateMs;
  uint16_t _curDelay;
  const Animation::AnimationInstance* _curAnimation = nullptr;
};

extern StatusLedService StatusLED;