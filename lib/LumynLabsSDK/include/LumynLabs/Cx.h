#pragma once
// =============================================================================
// ConnectorX SDK - ConnectorX
// Auto-generated entry point - DO NOT EDIT
// =============================================================================

#include <LumynLabs/Features.h>

// Core System API
#include <LumynLabs/System/SystemService.h>

// Event types
#include <LumynLabs/Eventing/EventType.h>
#include <LumynLabs/Eventing/Event.h>

// LED APIs
#if CX_FEATURE_LED
#include <LumynLabs/Led/Color.h>
#include <LumynLabs/Led/Animation.h>
#include <LumynLabs/Led/AnimationManager.h>
#endif

// Module APIs
#if CX_FEATURE_MODULES
#include <LumynLabs/Modules/Module.h>
#include <LumynLabs/Modules/ModuleConfig.h>
#include <LumynLabs/Modules/ModuleError.h>
#include <LumynLabs/Modules/ModulePeripherals.h>
#include <LumynLabs/Modules/ModuleRegistration.h>
#endif

