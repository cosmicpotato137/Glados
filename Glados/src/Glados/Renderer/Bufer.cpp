#include "gladospch.h"
#include "Buffer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Glados {


	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		return new OpenGLIndexBuffer(indices, size);
	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		return new OpenGLVertexBuffer(size);
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		return new OpenGLVertexBuffer(size);
	}
	 
	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size)
	{
		return new OpenGLUniformBuffer(size);
	}

	Ref<UniformBuffer> UniformBuffer::Create(float* vertices, uint32_t size)
	{
		return new OpenGLUniformBuffer(size);
	}

}