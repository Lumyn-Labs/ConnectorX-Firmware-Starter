#pragma once

#include <cstdint>
#include <memory>
#include <string_view>
#include <type_traits>

#include "lumyn/connectorx_domain.h"
#include "lumyn/domain/Color.h"
#include "lumyn/led/EffectContext.h"

// The types an animation is made of. They are public: custom firmware
// defines its own animations with them (the SDK names them LumynLabs::Effect
// and so on).
namespace lumyn::led
{
  // Effects move in fixed steps of 1/60 s of their own time. A command's
  // delay sets how fast that time runs (see AnimationPlayer), so an effect
  // never reads a clock and draws the same frames on a board and in the
  // simulator.
  inline constexpr int kTicksPerSecond = 60;

  // What an effect draws on: a zone's pixels, row by row from the top left.
  // A strip is a single row.
  struct Frame
  {
    Color *pixels;
    uint16_t width;
    uint16_t height;
    Color color;
    // Set only for effects that reverse themselves (kOwnsReverse).
    bool reversed;

    Color &at(int x, int y) { return pixels[y * width + x]; }
    int size() const { return width * height; }
  };

  // One zone's running animation. It keeps whatever state it needs, sized
  // for the zone it was made for.
  class Effect
  {
  public:
    virtual ~Effect() = default;
    // Moves on by one tick.
    virtual void step() = 0;
    // Draws the effect as it is now on a cleared frame.
    virtual void draw(Frame &frame) = 0;

  protected:
    // Random numbers from the effect's own generator. It starts as a copy of
    // the context current when the effect is made, which a player seeds for
    // its zone (see EffectContext), so a zone's frames depend only on its
    // own commands, and are the same on every platform.

    // 0 <= rnd() < 1.
    float rnd() { return static_cast<float>(_random.nextRandom() >> 8) * (1.0f / 16777216.0f); }
    float rnd(float lo, float hi) { return lo + rnd() * (hi - lo); }
    bool chance(float p) { return rnd() < p; }
    uint32_t random32() { return _random.nextRandom(); }
    // 0..n-1, for n up to 65536.
    int randomBelow(int n)
    {
      return static_cast<int>(((random32() >> 16) * static_cast<uint32_t>(n)) >> 16);
    }
    // True with the odds given by odds(p).
    bool happens(uint32_t chances) { return random32() < chances; }

  private:
    lumyn::internal::Animation::EffectContext _random =
        lumyn::internal::Animation::currentEffect();
  };

  enum AnimationFlag : uint8_t
  {
    // Needs a matrix: at least two rows and two columns.
    kMatrixOnly = 1,
    // Reversed changes the effect itself rather than flipping its pixels.
    kOwnsReverse = 2,
    // Played one-shot, it stops after one cycle. Other animations loop.
    kEnds = 4,
    // Its frame changes only when its color does.
    kStill = 8,
  };

  struct AnimationInstance
  {
    std::string_view id;
    uint16_t defaultDelay;
    Color defaultColor;
    // One cycle, in ticks at the default delay. A step of a sequence plays
    // for one cycle.
    uint16_t cycleTicks;
    uint8_t flags;
    // Makes the effect for a width x height zone. Its random numbers come
    // from the zone (see Effect).
    std::unique_ptr<Effect> (*make)(uint16_t width, uint16_t height);
  };

  // Whether `animation` can play on a width x height zone.
  inline bool fits(const AnimationInstance &animation, uint16_t width, uint16_t height)
  {
    return !(animation.flags & kMatrixOnly) || (width >= 2 && height >= 2);
  }

  // Makes an effect of type E, for AnimationInstance::make. E is made with
  // the zone's width and height, or with no arguments if it doesn't need
  // them.
  template <typename E>
  std::unique_ptr<Effect> makeEffect(uint16_t width, uint16_t height)
  {
    if constexpr (std::is_constructible_v<E, uint16_t, uint16_t>)
      return std::make_unique<E>(width, height);
    else
      return std::make_unique<E>();
  }

} // namespace lumyn::led

namespace lumyn::internal::Animation
{
  // Lumyn's own code names these types here too.
  using lumyn::led::AnimationFlag;
  using lumyn::led::AnimationInstance;
  using lumyn::led::Effect;
  using lumyn::led::fits;
  using lumyn::led::Frame;
  using lumyn::led::kEnds;
  using lumyn::led::kMatrixOnly;
  using lumyn::led::kOwnsReverse;
  using lumyn::led::kStill;
  using lumyn::led::kTicksPerSecond;
  using lumyn::led::makeEffect;

  // An animation's defaults, from the domain. Evaluated while compiling, so
  // the animations cost no code to set up.
  consteval uint16_t defaultDelay(connectorx_animation_t animation)
  {
    return connectorx_animation_default_delay(animation);
  }

  // The domain gives the color as 0xRRGGBB.
  consteval lumyn::led::Color defaultColor(connectorx_animation_t animation)
  {
    const uint32_t rgb = connectorx_animation_default_color(animation);
    return lumyn::led::Color((rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF);
  }

  // kMatrixOnly when the domain says the animation needs a matrix.
  consteval uint8_t matrixOnly(connectorx_animation_t animation)
  {
    return connectorx_animation_matrix_only(animation) ? kMatrixOnly : 0;
  }

  namespace detail
  {
    class Dark : public Effect
    {
    public:
      Dark(uint16_t, uint16_t) {}
      void step() override {}
      void draw(Frame &) override {}
    };
  } // namespace detail

  inline const AnimationInstance None = {
      .id = "NONE",
      .defaultDelay = defaultDelay(CONNECTORX_ANIMATION_NONE),
      .defaultColor = defaultColor(CONNECTORX_ANIMATION_NONE),
      .cycleTicks = 1,
      .flags = kStill,
      .make = makeEffect<detail::Dark>};
} // namespace lumyn::internal::Animation
