#pragma once

// Oops! If you ended up here, your code (or your editor's autocomplete) used
// a type from lumyn::internal. Those are Lumyn's own names, and they were
// removed for 2027. The compiler error tells you the name to use instead.
// It's the same type, so the fix is just the new name.
//
// Lumyn's own libraries build with LUMYN_INTERNAL_NAMES, which keeps the old
// names working inside them.
#ifdef LUMYN_INTERNAL_NAMES
#define LUMYN_INTERNAL_NAME(name, public_name, spelling) using name = public_name;
#else
#define LUMYN_INTERNAL_NAME(name, public_name, spelling)                                        \
  template <bool Removed = false>                                                               \
  struct name##RemovedFor2027 {                                                                 \
    static_assert(Removed, spelling " was removed for 2027. Use " #public_name " instead.");    \
  };                                                                                            \
  using name = name##RemovedFor2027<>;
#endif
