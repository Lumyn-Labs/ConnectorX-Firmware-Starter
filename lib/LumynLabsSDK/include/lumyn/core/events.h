/*
 * An event as a host program sees it, after decoding. The event IDs and the
 * enums its fields use come from the system domain (lumyn/domain_types.h).
 */
#pragma once

#include <stdint.h>

#include "lumyn/domain_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct lumyn_disabled_info {
  lumyn_disabled_cause_t cause;
} lumyn_disabled_info_t;

typedef struct lumyn_connected_info {
  lumyn_connection_type_t type;
} lumyn_connected_info_t;

typedef struct lumyn_disconnected_info {
  lumyn_connection_type_t type;
} lumyn_disconnected_info_t;

typedef struct lumyn_error_info {
  lumyn_error_type_t type;
  char message[32];  // the 16 wire bytes, zero-terminated
} lumyn_error_info_t;

typedef struct lumyn_fatal_error_info {
  lumyn_fatal_error_type_t type;
  char message[32];  // the 16 wire bytes, zero-terminated
} lumyn_fatal_error_info_t;

typedef struct lumyn_registered_entity_info {
  int32_t id;
} lumyn_registered_entity_info_t;

typedef struct lumyn_custom_info {
  uint8_t type;
  uint8_t data[16];
  uint8_t length;
} lumyn_custom_info_t;

typedef struct lumyn_pin_interrupt_info {
  uint8_t pin;
} lumyn_pin_interrupt_info_t;

typedef struct lumyn_heartbeat_info {
  lumyn_status_t status;
  uint8_t enabled;
  uint8_t connected_usb;
  uint8_t can_ok;
} lumyn_heartbeat_info_t;

typedef struct lumyn_event {
  lumyn_event_type_t type;
  union {
    lumyn_disabled_info_t disabled;
    lumyn_connected_info_t connected;
    lumyn_disconnected_info_t disconnected;
    lumyn_error_info_t error;
    lumyn_fatal_error_info_t fatal_error;
    lumyn_registered_entity_info_t registered_entity;
    lumyn_custom_info_t custom;
    lumyn_pin_interrupt_info_t pin_interrupt;
    lumyn_heartbeat_info_t heartbeat;
  } data;
  // Optional text, or NULL. Whether you must free it depends on the function
  // that gave you the event; each one says.
  char* extra_message;
} lumyn_event_t;

#ifdef __cplusplus
} // extern "C"

#endif // __cplusplus
