#pragma once
// =============================================================================
// ConnectorX SDK - Feature Flags
// Auto-generated for ConnectorX - DO NOT EDIT
// =============================================================================

// SDK Version
#define CX_SDK_VERSION_MAJOR 1
#define CX_SDK_VERSION_MINOR 0
#define CX_SDK_VERSION_PATCH 0
#define CX_SDK_VERSION_STRING "1.0.0"

// Board Variant Identifier
#define CX_SDK_VARIANT "cx"
#define CX_SDK_VARIANT_NAME "ConnectorX"
#define CX_SDK_VARIANT_VERSION "1.0.0"

// Feature Flags - use these in #if directives
#define CX_FEATURE_LED 1
#define CX_FEATURE_MODULES 1
#define CX_FEATURE_SCREEN 0
#define CX_FEATURE_SD_CARD 1
#define CX_FEATURE_INDICATORS 1
#define CX_FEATURE_CAN_BUS 0
#define CX_FEATURE_DIO 1
#define CX_FEATURE_AIO 1

// LED Configuration (safe to expose - no pin info)
#define CX_LED_CHANNEL_COUNT 1
#define CX_LED_POWER_DRAW_MA 60

// Screen Configuration (safe to expose - no addresses)
#define CX_SCREEN_WIDTH_PX 128
#define CX_SCREEN_HEIGHT_PX 64

// Power Configuration
#define CX_REGULATOR_OUTPUT_MA 3000

// SDK marker - used by internal code to detect SDK builds
#define LUMYN_SDK 1

