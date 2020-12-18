#include "gladospch.h"
#include "Renderer.h"
#include "GraphicsContext.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Glados {

	// TODO: make Glados::Scope?
	GraphicsContext* GraphicsContext::Create(void* windowHandle)
	{
		switch (Renderer::GetRenderAPI())
		{
		case RendererAPI::API::None:	GD_CORE_ASSERT(false, "RendererAPI::API::None is not currently supported!");
		case RendererAPI::API::OpenGL:	return new OpenGLContext((GLFWwindow*)windowHandle);
		}
		GD_CORE_ASSERT(false, "Unknown RendererAPI::API!");
		return nullptr;
	}

}