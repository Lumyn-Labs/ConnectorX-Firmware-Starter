#pragma once

#include <cstdint>

#ifdef ARDUINO
extern "C" {
void vTaskEnterCritical(void);
void vTaskExitCritical(void);
}
#define REFCOUNT_ENTER_CRITICAL() vTaskEnterCritical()
#define REFCOUNT_EXIT_CRITICAL() vTaskExitCritical()
#else
#include <atomic>
#define REFCOUNT_ENTER_CRITICAL() do { } while (0)
#define REFCOUNT_EXIT_CRITICAL() do { } while (0)
#endif

namespace lumyn::internal {

class RefCounted {
 public:
  void ref() const {
    REFCOUNT_ENTER_CRITICAL();
    ++_refCount;
    REFCOUNT_EXIT_CRITICAL();
  }

  virtual void unref() const {
    REFCOUNT_ENTER_CRITICAL();
    const uint32_t count = --_refCount;
    REFCOUNT_EXIT_CRITICAL();
    if (count == 0) delete this;
  }

  uint32_t getRefCount() const {
    REFCOUNT_ENTER_CRITICAL();
    const uint32_t count = _refCount;
    REFCOUNT_EXIT_CRITICAL();
    return count;
  }

 protected:
  RefCounted() : _refCount(1) {}
  virtual ~RefCounted() = default;

  bool decrementRefCount() const {
    REFCOUNT_ENTER_CRITICAL();
    const uint32_t count = --_refCount;
    REFCOUNT_EXIT_CRITICAL();
    return count == 0;
  }

 private:
#ifdef ARDUINO
  mutable uint32_t _refCount;
#else
  mutable std::atomic<uint32_t> _refCount;
#endif
};

}  // namespace lumyn::internal
