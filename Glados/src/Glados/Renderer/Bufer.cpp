#include "gladospch.h"

#include "Buffer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Glados {

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (GD_GRAPHICS_API)
		{
		case GD_OPENGL_API:	return CreateRef<OpenGLIndexBuffer>(indices, count);
		}
		GD_CORE_ASSERT(false, "Unknown API!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(size_t size)
	{
		switch (GD_GRAPHICS_API)
		{
		case GD_OPENGL_API:	return CreateRef<OpenGLVertexBuffer>(size);
		}
		GD_CORE_ASSERT(false, "Unknown API!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, size_t size)
	{
		switch (GD_GRAPHICS_API)
		{
		case GD_OPENGL_API:	return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}
		GD_CORE_ASSERT(false, "Unknown API!");
		return nullptr;
	}
	 
	Ref<UniformBuffer> UniformBuffer::Create(size_t size)
	{
		switch (GD_GRAPHICS_API)
		{
		case GD_OPENGL_API:	return CreateRef<OpenGLUniformBuffer>(size);
		}
		GD_CORE_ASSERT(false, "Unknown API!");
		return nullptr;
	}

	Ref<UniformBuffer> UniformBuffer::Create(float* vertices, size_t size)
	{
		switch (GD_GRAPHICS_API)
		{
		case GD_OPENGL_API:	return CreateRef<OpenGLUniformBuffer>(vertices, size);
		}
		GD_CORE_ASSERT(false, "Unknown API!");
		return nullptr;
	}

}