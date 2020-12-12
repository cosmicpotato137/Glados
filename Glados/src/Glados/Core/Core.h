#pragma once
#include <memory>

// supported rendering APIs
#define GD_OPENGL_API 1

// supported window APIs
#define GD_GLFW_API 1

// platform specific stuff
#ifdef GD_PLATFORM_WINDOWS
#define GD_GRAPHICS_API GD_OPENGL_API
#define GD_WINDOW_API GD_GLFW_API

#else
#define GD_GRAPHICS_API 0
#define GD_WINDOW_API 0
#error Glados only supports Windows!
#endif

// event bind macro
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

// bitfield
#define BIT(x) (1 << x)

namespace Glados {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}

#include "GDLog.h"