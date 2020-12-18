#include "gladospch.h"
#include "Renderer.h"
#include "Texture.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Glados {

	Glados::Ref<Texture> Texture::Create(const std::string& filepath)
	{
		switch (Renderer::GetRenderAPI())
		{
		case RendererAPI::API::None:	GD_CORE_ASSERT(false, "RendererAPI::API::None is not currently supported!");
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture>(filepath);
		}
		GD_CORE_ASSERT(false, "Unknown RendererAPI::API!");
		return nullptr;
	}

}