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

#define BIT(x) (1 << x)