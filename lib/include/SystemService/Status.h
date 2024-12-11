#pragma once

#include "definitions/domain/event/EventType.h"

namespace SystemStatus {
static const std::map<Eventing::EventType, Eventing::Status> kEventStatuses = {
    {Eventing::EventType::BeginInitialization, Eventing::Status::Booting},
    {Eventing::EventType::Error, Eventing::Status::Error},
    {Eventing::EventType::FatalError, Eventing::Status::Fatal}};

static std::map<Eventing::Status, Command::LED::LEDCommand> kStatusLedAnimations = {
    {Eventing::Status::Booting,
     {.type = Command::LED::LEDCommandType::SetAnimation,
      .setAnimation = {.zoneId = 0,
                       .animationId = IDCreator::createId(std::string("Blink")),
                       .delay = 250,
                       .color = {.r = 0, .g = 255, .b = 50},
                       .oneShot = false}}},
    {Eventing::Status::Active,
     {.type = Command::LED::LEDCommandType::SetAnimation,
      .setAnimation = {.zoneId = 0,
                       .animationId =
                           IDCreator::createId(std::string("Breathe")),
                       .delay = 10,
                       .color = {.r = 0, .g = 20, .b = 200},
                       .oneShot = false}}},
    {Eventing::Status::Error,
     {.type = Command::LED::LEDCommandType::SetAnimation,
      .setAnimation = {.zoneId = 0,
                       .animationId = IDCreator::createId(std::string("Blink")),
                       .delay = 500,
                       .color = {.r = 120, .g = 30, .b = 5},
                       .oneShot = false}}},
    {Eventing::Status::Fatal,
     {.type = Command::LED::LEDCommandType::SetAnimation,
      .setAnimation = {.zoneId = 0,
                       .animationId = IDCreator::createId(std::string("Blink")),
                       .delay = 250,
                       .color = {.r = 255, .g = 0, .b = 0},
                       .oneShot = false}}}};
}  // namespace SystemStatus