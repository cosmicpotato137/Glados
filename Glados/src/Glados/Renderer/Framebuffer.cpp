#include "gladospch.h"
#include "Renderer.h"
#include "Framebuffer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Glados {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetRenderAPI())
		{
		case RendererAPI::API::None:	GD_CORE_ASSERT(false, "RendererAPI::API::None is not currently supported!");
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLFramebuffer>(spec);
		}
		GD_CORE_ASSERT(false, "Unknown RendererAPI::API!");
		return nullptr;
	}

}

