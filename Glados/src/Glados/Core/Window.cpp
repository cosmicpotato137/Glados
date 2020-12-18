#include "gladospch.h"
#include "Window.h"

#ifdef GD_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Glados {

	// TODO: Make scope?
	Window* Window::Create(const WindowProps& props)
	{
#ifdef GD_PLATFORM_WINDOWS
		return new WindowsWindow(props);
#else
		GD_CORE_ASSERT(false, "Unknown Platform!");
		return nullptr;
#endif
	}

}