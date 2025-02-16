#pragma once

#include <stdint.h>

#include <string>

#include "definitions/domain/event/Event.h"
#include "definitions/domain/request/Request.h"
#include "definitions/domain/request/RequestType.h"

struct RuntimeContext {
  Request::HostConnectionSource connSource;
  Eventing::Status status;
  Eventing::ErrorFlags errorFlags;
  bool connected;
  bool settingsValid;
  std::string assignedId;
  uint32_t lastHeartbeatTs;
};

extern RuntimeContext SystemContext;