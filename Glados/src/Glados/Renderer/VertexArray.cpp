#include "gladospch.h"
#include "Renderer.h"
#include "VertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Glados {

	Ref<Glados::VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetRenderAPI())
		{
		case RendererAPI::API::None:	GD_CORE_ASSERT(false, "RendererAPI::API::None is not currently supported!");
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexArray>();
		}
		GD_CORE_ASSERT(false, "Unknown RendererAPI::API!");
		return nullptr;
	}

}