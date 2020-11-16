#pragma once

#ifdef GD_PLATFORM_WINDOWS
	#ifdef GD_BUILD_DLL
		#define GLADOS_API __declspec(dllexport)
	#else
		#define GLADOS_API __declspec(dllimport)
	#endif
#else
	#error Glados only supports Windows!
#endif

#ifdef GD_ENABLE_ASSERTS
#define GD_ASSERT(x, ...) {if(!(x)) { GD_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define GD_CORE_ASSERT(x, ...) {if(!(x)) { GD_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define GD_ASSERT(x, ...)
#define GD_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)