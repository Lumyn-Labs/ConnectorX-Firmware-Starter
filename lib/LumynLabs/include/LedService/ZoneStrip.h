#pragma once

#include "LedService/Zone.h"
#include "definitions/led/AnimationSequenceInstance.h"

class ZoneStrip : public Zone {
 public:
  ZoneStrip(const Configuration::Zone&, CRGB*);

  bool update(void) override;

  void setAnimation(const Animation::AnimationInstance* action, bool oneShot);

  void setAnimation(const Animation::AnimationInstance* action, uint16_t delay,
                    Configuration::ActionColor color, bool reversed,
                    bool oneShot);

  void setAnimationSequence(
      const Animation::AnimationSequenceInstance* definition);

 protected:
  bool incrementState(void);

  void resetSequence(void);

  void setAnimationWithoutSequenceClearing(
      const Animation::AnimationInstance* action, uint16_t delay,
      Configuration::ActionColor color, bool reversed, bool oneShot);
  void moveToSequenceStep(uint16_t index);
  void moveToNextSequenceStep(void);

  uint16_t _sequenceStepIndex;
  uint16_t _sequenceRepeatCount;
  bool _reversed;
  const Animation::AnimationSequenceInstance* _curAnimationSequence = nullptr;
  const Animation::AnimationInstance* _curAnimation = nullptr;
};