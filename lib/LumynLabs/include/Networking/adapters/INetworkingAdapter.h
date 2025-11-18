#pragma once

#include <Arduino.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>

#include <functional>
#include <memory>
#include <string>

#include "lumyn/domain/transmission/Transmission.h"

namespace Transmission = lumyn::internal::Transmission;

namespace Networking {
class INetworkingAdapter {
 public:
  virtual void init(uint16_t transmissionQueueSize) = 0;
  virtual void start(uint32_t stackSize, UBaseType_t priority) = 0;
  // ! This MUST call delete on the body's vector!
  virtual void writeTransmission(Transmission::Transmission* transmission) = 0;
  virtual void onTransmission(
      std::function<void(Transmission::Transmission*)>) = 0;
  virtual const char* name(void) const = 0;
  virtual uint32_t maxMTU(void) const = 0;
};
}  // namespace Networking