#include "gladospch.h"

#include "Buffer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Glados {

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (GD_GRAPHICS_API)
		{
		case GD_OPENGL_API:	return CreateRef<OpenGLIndexBuffer>(indices, size);
		}
		GD_CORE_ASSERT(false, "Unknown API!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (GD_GRAPHICS_API)
		{
		case GD_OPENGL_API:	return CreateRef<OpenGLVertexBuffer>(size);
		}
		GD_CORE_ASSERT(false, "Unknown API!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (GD_GRAPHICS_API)
		{
		case GD_OPENGL_API:	return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}
		GD_CORE_ASSERT(false, "Unknown API!");
		return nullptr;
	}
	 
	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size)
	{
		switch (GD_GRAPHICS_API)
		{
		case GD_OPENGL_API:	return CreateRef<OpenGLUniformBuffer>(size);
		}
		GD_CORE_ASSERT(false, "Unknown API!");
		return nullptr;
	}

	Ref<UniformBuffer> UniformBuffer::Create(float* vertices, uint32_t size)
	{
		switch (GD_GRAPHICS_API)
		{
		case GD_OPENGL_API:	return CreateRef<OpenGLUniformBuffer>(vertices, size);
		}
		GD_CORE_ASSERT(false, "Unknown API!");
		return nullptr;
	}

}