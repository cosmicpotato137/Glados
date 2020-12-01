#include "gladospch.h"

#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Glados {

	Scope<RendererAPI> RendererAPI::Create(API api)
	{
		switch (api)
		{
		case API::OpenGL:	return CreateScope<OpenGLRendererAPI>();
		}
		GD_CORE_ASSERT(false, "Unknown API!");
		return nullptr;
	}

}