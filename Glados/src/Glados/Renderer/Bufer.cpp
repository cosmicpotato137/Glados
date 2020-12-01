#include "gladospch.h"

#include "Buffer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Glados {

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		return CreateRef<OpenGLIndexBuffer>(indices, size);
	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		return CreateRef<OpenGLVertexBuffer>(size);
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		return CreateRef<OpenGLVertexBuffer>(vertices, size);
	}
	 
	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size)
	{
		return CreateRef<OpenGLUniformBuffer>(size);
	}

	Ref<UniformBuffer> UniformBuffer::Create(float* vertices, uint32_t size)
	{
		return CreateRef<OpenGLUniformBuffer>(vertices, size);
	}

}