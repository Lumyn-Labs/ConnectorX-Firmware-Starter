#pragma once

#include <stdint.h>

#include <string>

#include "lumyn/domain/event/Event.h"
#include "lumyn/domain/request/Request.h"
#include "lumyn/domain/request/RequestType.h"

namespace Eventing = lumyn::internal::Eventing;
namespace Request = lumyn::internal::Request;

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