#pragma once

#include "lumyn/util/hashing/IDCreator.h"
#include "lumyn/domain/command/Command.h"
#include "lumyn/domain/event/EventType.h"

namespace Eventing = lumyn::internal::Eventing;
namespace Command = lumyn::internal::Command;

namespace SystemStatus {
static const std::map<Eventing::EventType, Eventing::Status> kEventStatuses = {
    {Eventing::EventType::BeginInitialization, Eventing::Status::Booting},
    {Eventing::EventType::Error, Eventing::Status::Error},
    {Eventing::EventType::FatalError, Eventing::Status::Fatal}};

static std::map<Eventing::Status, Command::LED::LEDCommand>
    kStatusLedAnimations = {
        {Eventing::Status::Booting,
         {Command::LED::LEDCommandType::SetAnimation,
          {{
              0,  // zoneId
              lumyn::internal::IDCreator::createId(std::string("Blink")),
              250,  // delay
              {0, 255, 50},  // color
              0,  // reversed
              0   // oneShot
          }}}},
        {Eventing::Status::Active,
         {Command::LED::LEDCommandType::SetAnimation,
          {{
              0,  // zoneId
              lumyn::internal::IDCreator::createId(std::string("Breathe")),
              10,  // delay
              {0, 20, 200},  // color
              0,  // reversed
              0   // oneShot
          }}}},
        {Eventing::Status::Error,
         {Command::LED::LEDCommandType::SetAnimation,
          {{
              0,  // zoneId
              lumyn::internal::IDCreator::createId(std::string("Blink")),
              500,  // delay
              {120, 30, 5},  // color
              0,  // reversed
              0   // oneShot
          }}}},
        {Eventing::Status::Fatal,
         {Command::LED::LEDCommandType::SetAnimation,
          {{
              0,  // zoneId
              lumyn::internal::IDCreator::createId(std::string("Blink")),
              250,  // delay
              {255, 0, 0},  // color
              0,  // reversed
              0   // oneShot
          }}}}
};
}  // namespace SystemStatus