#include "gladospch.h"
#include "Renderer.h"
#include "Texture.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Glados {

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetRenderAPI())
		{
		case RendererAPI::API::None:    GD_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(width, height);
		}

		GD_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetRenderAPI())
		{
		case RendererAPI::API::None:    GD_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(path);
		}

		GD_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}