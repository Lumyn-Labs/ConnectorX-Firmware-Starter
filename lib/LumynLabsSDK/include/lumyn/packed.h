#pragma once

#ifdef __cplusplus
#include <bit>
static_assert(std::endian::native == std::endian::little,
              "Lumyn wire structs require a little-endian target");
#endif

#ifdef __GNUC__
#define PACK(__Declaration__) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PACK(__Declaration__) \
  __pragma(pack(push, 1)) __Declaration__ __pragma(pack(pop))
#endif
