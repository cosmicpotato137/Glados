#include "gladospch.h"
#include "Renderer.h"
#include "Buffer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Glados {

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetRenderAPI())
		{
		case RendererAPI::API::None:	GD_CORE_ASSERT(false, "RendererAPI::API::None is not currently supported!");
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLIndexBuffer>(indices, count);
		}
		GD_CORE_ASSERT(false, "Unknown RendererAPI::API!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(size_t size)
	{
		switch (Renderer::GetRenderAPI())
		{
		case RendererAPI::API::None:	GD_CORE_ASSERT(false, "RendererAPI::API::None is not currently supported!");
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexBuffer>(size);
		}
		GD_CORE_ASSERT(false, "Unknown RendererAPI::API!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, size_t size)
	{
		switch (Renderer::GetRenderAPI())
		{
		case RendererAPI::API::None:	GD_CORE_ASSERT(false, "RendererAPI::API::None is not currently supported!");
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}
		GD_CORE_ASSERT(false, "Unknown RendererAPI::API!");
		return nullptr;
	}
	 
	Ref<UniformBuffer> UniformBuffer::Create(size_t size)
	{
		switch (Renderer::GetRenderAPI())
		{
		case RendererAPI::API::None:	GD_CORE_ASSERT(false, "RendererAPI::API::None is not currently supported!");
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLUniformBuffer>(size);
		}
		GD_CORE_ASSERT(false, "Unknown RendererAPI::API!");
		return nullptr;
	}

	Ref<UniformBuffer> UniformBuffer::Create(float* vertices, size_t size)
	{
		switch (Renderer::GetRenderAPI())
		{
		case RendererAPI::API::None:	GD_CORE_ASSERT(false, "RendererAPI::API::None is not currently supported!");
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLUniformBuffer>(vertices, size);
		}
		GD_CORE_ASSERT(false, "Unknown RendererAPI::API!");
		return nullptr;
	}

}