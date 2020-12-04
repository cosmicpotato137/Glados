#include "gladospch.h"
#include "Window.h"
#include "Platform/Windows/WindowsWindow.h"

namespace Glados {

	Window* Window::Create(const WindowProps& props)
	{
		switch (GD_WINDOW_API)
		{
		case GD_GLFW_API: return new WindowsWindow(props);
		}
		GD_CORE_ASSERT(false, "Unknown API!");
		return nullptr;
	}

}