#pragma once

#include <Arduino.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>

#include <map>
#include <memory>
#include <optional>

#include "ChannelGetter.h"
#include "ConfigurationParser/Configuration.h"
#include "Eventing/EventingService.h"
#include "LedService/AnimationGroupManager.h"
#include "LedService/AnimationManager.h"
#include "LedService/AnimationSequenceManager.h"
#include "LedService/BitmapManager.h"
#include "LedService/Channel.h"
#include "lumyn/domain/command/led/LEDCommand.h"

namespace Command = lumyn::internal::Command;

/**
 * The LEDService maintains ownership of Channels and Zones
 *
 * A Channel can reference n Zones as their "owner"; it also holds the strip's
 * CRGB buffer and FastLED CLEDController reference
 *
 * A Zone is either a ZoneStrip or ZoneMatrix:
 *  ZoneStrip - Runs a single AnimationInstance on its view of the CRGB
 * buffer from the Channel; can also run a AnimationSequenceInstance if
 * requested ZoneMatrix - Runs a single BitmapInstance on its view of the CRGB
 * buffer from the Channel
 *
 * The AnimationManager maintains ownership of all PatternActionInstances
 *
 * A AnimationInstance describes a pattern that can be executed by a Zone.
 * An external request will use it either by itself or in conjunction with a
 * Animation configuration to set the delay, color, and reversal state
 *
 * The PatternManager maintains ownership of all PatternInstances
 *
 * A AnimationSequenceInstance is typically created from a Animation
 * configuration and can be executed by a Zone. It maintains a list of
 * references to both the PatternActionInstances AND the Animation
 * configurations
 *
 * The BitmapManager maintains ownership of all BitmapInstances
 *
 * Each one includes a reference to the relevant Bitmap configuration and the
 * number of frames: 1 for a static type and 1 to N for an animated type
 *
 */

class LEDService {
 public:
  enum class LEDError {
    Ok = 1,
    InvalidOrMissingZone = -1,
    MissingAnimation = -2,
    MissingGroup = -3,
    MissingSequence = -4,
    MissingBitmap = -5,
  };

  LEDService(uint8_t maxQueueElements = 50,
             TickType_t queueTimeout = pdMS_TO_TICKS(200));

  void init(const Configuration::CXv3Configuration* config);
  void start(uint32_t stackSize, UBaseType_t priority);

  // ! MUST be called when running as a separate Task
  void sendCommand(Command::LED::LEDCommand* cmd) const {
    xQueueSend(_commandQueue, cmd, _queueTimeout);
  }

  uint16_t registerChannel(LEDChannelPort port,
                           const Configuration::Channel& config,
                           uint16_t totalLedCount, bool hidden = false);
  uint16_t registerAnimationSequence(const Configuration::AnimationSequence&);
  uint16_t registerAnimationGroup(const Configuration::AnimationGroup&);
  uint16_t registerBitmap(const Configuration::Bitmap&);

  void setAnimationAsync(std::string_view zoneId, std::string_view animationId,
                         uint16_t delay, Command::LED::AnimationColor color,
                         bool reversed, bool oneShot);
  void setAnimationSequenceAsync(std::string_view zoneId,
                                 std::string_view sequenceId);
  void setAnimationGroupAsync(std::string_view groupId,
                              std::string_view animationId, uint16_t delay,
                              Command::LED::AnimationColor color, bool reversed,
                              bool oneShot);
  void setAnimationSequenceGroupAsync(std::string_view groupId,
                                      std::string_view sequenceId);
  void setColorAsync(std::string_view zoneId,
                     Command::LED::AnimationColor color);
  void setColorGroupAsync(std::string_view groupId,
                          Command::LED::AnimationColor color);
  void setBitmapAsync(std::string_view zoneId, std::string_view bitmapId,
                      std::optional<Command::LED::AnimationColor> color,
                      bool oneShot);
  void setBitmapGroupAsync(std::string_view groupId, std::string_view bitmapId,
                           std::optional<Command::LED::AnimationColor> color,
                           bool oneShot);
  void setMatrixTextAsync(std::string_view zoneId,
                          Command::LED::AnimationColor color,
                          Command::LED::MatrixTextScrollDirection direction,
                          std::string_view text, uint8_t length, uint16_t delay,
                          bool oneShot = false);
  void setMatrixGroupTextAsync(
      std::string_view groupId, Command::LED::AnimationColor color,
      Command::LED::MatrixTextScrollDirection direction, std::string_view text,
      uint8_t length, uint16_t delay, bool oneShot = false);

  std::optional<const Channel*> getChannel(std::string_view id);
  std::optional<Zone*> getZone(std::string_view id);
  std::optional<const Channel*> getChannel(uint16_t id);
  std::optional<Zone*> getZone(uint16_t id);
  std::optional<Channel*> getChannelForZone(uint16_t zoneId);

  LEDError setZoneBuffer(uint16_t zoneId, uint8_t* buf, uint16_t count);

  inline const AnimationManager& animationManager() const {
    return AnimationMngr;
  }

  inline const AnimationSequenceManager& animationSequenceManager() const {
    return *_sequenceManager;
  }

  inline const AnimationGroupManager& animationGroupManager() const {
    return *_groupManager;
  }

  inline const BitmapManager& bitmapManager() const { return *_bitmapManager; }

  static Eventing::Event createEventFromLedError(LEDError err,
                                                 const std::string& name);

 private:
  static void initImpl(void* _this);
  void task(void);
  void handleCommand(Command::LED::LEDCommand& command);
  void sendEventFromLEDError(LEDError err, const std::string& name);

  LEDError setAnimation(uint16_t zoneId, uint16_t animationId, uint16_t delay,
                        Configuration::ActionColor color, bool reversed,
                        bool oneShot);
  LEDError setAnimationSequence(uint16_t zoneId, uint16_t sequenceId);
  LEDError setAnimationGroup(uint16_t groupId, uint16_t animationId,
                             uint16_t delay, Configuration::ActionColor color,
                             bool reversed, bool oneShot);
  LEDError setAnimationSequenceGroup(uint16_t groupId, uint16_t sequenceId);
  LEDError setColor(uint16_t zoneId, Configuration::ActionColor color);
  LEDError setColorGroup(uint16_t groupId, Configuration::ActionColor color);
  LEDError setBitmap(uint16_t zoneId, uint16_t bitmapId,
                     std::optional<Configuration::ActionColor> color,
                     bool oneShot);
  LEDError setBitmapGroup(uint16_t groupId, uint16_t bitmapId,
                          std::optional<Configuration::ActionColor> color,
                          bool oneShot);
  LEDError setMatrixText(uint16_t zoneId, Configuration::ActionColor color,
                         Command::LED::MatrixTextScrollDirection direction,
                         const char* text, uint8_t length, uint16_t delay,
                         bool oneShot = false);
  LEDError setMatrixGroupText(uint16_t groupId,
                              Configuration::ActionColor color,
                              Command::LED::MatrixTextScrollDirection direction,
                              const char* text, uint8_t length, uint16_t delay,
                              bool oneShot = false);

  const Configuration::CXv3Configuration* _config = nullptr;
  std::map<uint16_t, std::unique_ptr<Channel>> _channels;
  std::map<uint16_t, std::pair<Zone*, Channel*>> _zones;
  FileService** _fileService = &FileAccessService;
  EventingService& _eventService = EventService;
  uint8_t _maxQueueElements;
  TickType_t _queueTimeout;
  TaskHandle_t _task;
  QueueHandle_t _commandQueue;
  std::unique_ptr<AnimationSequenceManager> _sequenceManager;
  std::unique_ptr<AnimationGroupManager> _groupManager;
  std::unique_ptr<BitmapManager> _bitmapManager;
};

extern LEDService LedService;