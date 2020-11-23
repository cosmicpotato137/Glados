#pragma once

// platform specific stuff
#ifdef GD_PLATFORM_WINDOWS

#else
#error Glados only supports Windows!
#endif

// debug assertions
#ifdef GD_ENABLE_ASSERTS
#define GD_ASSERT(x, ...) {if(!(x)) { GD_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define GD_CORE_ASSERT(x, ...) {if(!(x)) { GD_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define GD_ASSERT(x, ...)
#define GD_CORE_ASSERT(x, ...)
#endif

// event bind macro
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

// bitfield
#define BIT(x) (1 << x)