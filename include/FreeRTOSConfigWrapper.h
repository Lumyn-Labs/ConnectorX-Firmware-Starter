#ifndef FREERTOS_CONFIG_WRAPPER_H
#define FREERTOS_CONFIG_WRAPPER_H

// This file acts as a wrapper around the real FreeRTOSConfig.h
// We'll include it from the framework after defining our overrides

// First, include the real FreeRTOSConfig.h from the pico-sdk
// The framework stores it in a predictable location
#if defined(ARDUINO_ARCH_RP2040)
// For RP2040, the config is in the pico-sdk
// We need to skip our own file and include the framework's version
#pragma push_macro("FREERTOS_CONFIG_WRAPPER_H")
#undef FREERTOS_CONFIG_WRAPPER_H

// Include from the framework's FreeRTOS path
#include_next "FreeRTOSConfig.h"

#pragma pop_macro("FREERTOS_CONFIG_WRAPPER_H")
#else
#error "Unsupported architecture"
#endif

// Now override the settings we want to change
#ifdef __cplusplus
extern "C" {
#endif
void vAssertCalled(const char* file, int line);
#ifdef __cplusplus
}
#endif

// Override stack overflow checking
#ifdef configCHECK_FOR_STACK_OVERFLOW
#undef configCHECK_FOR_STACK_OVERFLOW
#endif
#define configCHECK_FOR_STACK_OVERFLOW 2

// Override malloc hook
#ifdef configUSE_MALLOC_FAILED_HOOK
#undef configUSE_MALLOC_FAILED_HOOK
#endif
#define configUSE_MALLOC_FAILED_HOOK 1

// Override assert macro
#ifdef configASSERT
#undef configASSERT
#endif
#define configASSERT(x) \
  if ((x) == 0) vAssertCalled(__FILE__, __LINE__)

#endif  // FREERTOS_CONFIG_WRAPPER_H