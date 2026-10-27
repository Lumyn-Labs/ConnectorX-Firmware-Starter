#pragma once

// A zone's own random numbers. Each AnimationPlayer has one, and makes it
// current while it makes an effect, which starts its own generator from it
// (see Effect). Effects made outside a player start from one fallback
// context. The current context is one pointer for the program, so players on
// different threads must not make effects at the same time.

#include <cstdint>

namespace lumyn::internal::Animation {

class EffectContext {
 public:
  explicit EffectContext(uint32_t seed = 1) { reset(seed); }

  // Restarts the random numbers from `seed`.
  void reset(uint32_t seed) { _random = seed != 0 ? seed : 1; }

  // xorshift32: the same numbers on every platform for the same seed.
  uint32_t nextRandom() {
    _random ^= _random << 13;
    _random ^= _random >> 17;
    _random ^= _random << 5;
    return _random;
  }

  static EffectContext*& current() {
    static EffectContext* context = nullptr;
    return context;
  }

 private:
  uint32_t _random = 1;
};

// The context effects use: the drawing player's, or the fallback.
inline EffectContext& currentEffect() {
  static EffectContext fallback;
  EffectContext* context = EffectContext::current();
  return context ? *context : fallback;
}

// Makes `context` current for as long as this lives.
class ScopedEffect {
 public:
  explicit ScopedEffect(EffectContext& context)
      : _previous(EffectContext::current()) {
    EffectContext::current() = &context;
  }
  ~ScopedEffect() { EffectContext::current() = _previous; }
  ScopedEffect(const ScopedEffect&) = delete;
  ScopedEffect& operator=(const ScopedEffect&) = delete;

 private:
  EffectContext* _previous;
};

}  // namespace lumyn::internal::Animation
