#include "gladospch.h"

#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Glados {

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (GD_GRAPHICS_API)
		{
		case GD_OPENGL_API:	return CreateScope<OpenGLRendererAPI>();
		}
		GD_CORE_ASSERT(false, "Unknown API!");
		return nullptr;
	}

}