#include "gladospch.h"
#include "UniformBuffer.h"

#include "Glados/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace Glados{

	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::GetRenderAPI())
		{
			case RendererAPI::API::None:    GD_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLUniformBuffer>(size, binding);
		}

		GD_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
