#include "gladospch.h"
#include "GraphicsContext.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Glados {

	GraphicsContext* GraphicsContext::Create(void* windowHandle)
	{
		switch (GD_GRAPHICS_API)
		{
			case GD_OPENGL_API:	return new OpenGLContext((GLFWwindow*)windowHandle);
		}
		GD_CORE_ASSERT(false, "Unknown API!");
		return nullptr;
	}

}