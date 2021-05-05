#include "gladospch.h"
#include "OpenGLBuffer.h"
#include "glad/glad.h"

namespace Glados {

	//-----------------------------------
	//
	// IndexBuffer
	//
	//-----------------------------------

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	uint32_t OpenGLIndexBuffer::GetCount() const
	{
		return m_Count;
	}

	//-----------------------------------
	//
	// VertexBuffer
	//
	//-----------------------------------

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, size_t size)
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(size_t size)
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t offset, size_t size)
	{
		Bind();
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
		Unbind();
	}

	float OpenGLVertexBuffer::GetData(uint32_t offset, size_t size) const
	{
		float data;
		Bind();
		glGetBufferSubData(GL_ARRAY_BUFFER, offset, size, &data);
		Unbind();
		return data;
	}

	void OpenGLVertexBuffer::SetLayout(const BufferLayout& layout)
	{
		m_Layout = layout;
	}

	BufferLayout OpenGLVertexBuffer::GetLayout() const
	{
		return m_Layout;
	}

}