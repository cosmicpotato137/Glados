#pragma once
#include "GDLog.h"

// debug assertions
#ifdef GD_ENABLE_ASSERTS
#define GD_ASSERT(x, ...) {if(!(x)) { GD_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define GD_CORE_ASSERT(x, ...) {if(!(x)) { GD_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define GD_ASSERT(x, ...)
#define GD_CORE_ASSERT(x, ...)
#endif
