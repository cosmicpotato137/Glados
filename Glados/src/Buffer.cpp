#include "gladospch.h"

#include "Buffer.h"


namespace Glados {

	//-----------------------------------
	//
	// VertexBuffer
	//
	//-----------------------------------

	VertexBuffer::VertexBuffer(const VertexBufferLayout& layout, unsigned int numelts, const void* data, unsigned int usage)
		: Buffer(layout, numelts)
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, numelts * layout.ByteSize(), data, usage));
	}

	VertexBuffer::~VertexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_RendererID));
	}

	void VertexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	}

	void VertexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	//-----------------------------------
	//
	// IndexBuffer
	//
	//-----------------------------------

	IndexBuffer::IndexBuffer(const IndexBufferLayout& layout, unsigned int numelts, const unsigned int* data, unsigned int usage)
		: Buffer(layout, numelts)
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, numelts * sizeof(unsigned int), data, usage));
	}

	IndexBuffer::~IndexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_RendererID));
	}

	void IndexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	}

	void IndexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	//-----------------------------------
	//
	// UniformBuffer
	//
	//-----------------------------------

	UniformBuffer::UniformBuffer(const UniformBufferLayout& layout, unsigned int numelts, unsigned int binding, const void* data, unsigned int usage)
		: Buffer(layout, numelts), m_Binding(binding)
	{
		// create the uniform BUFFER and bind it to the correct binding point
		GLCall(glGenBuffers(1, &m_RendererID));
		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_UNIFORM_BUFFER, layout.ByteSize() * numelts, NULL, GL_DYNAMIC_DRAW));
		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));

		GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID));
		//UniformBuffer::m_UBB[binding] = m_RendererID;
	}

	UniformBuffer::~UniformBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_RendererID));
		//UniformBuffer::m_UBB.erase(m_Binding);
	}

	void UniformBuffer::BindUniformBlock(const std::string& name, std::shared_ptr<Shader> shader)
	{
		shader->SetUniformBlockIndex(name, m_Binding);
	}

	void UniformBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	}

	void UniformBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void UniformBuffer::SetBufferSubData(unsigned int x, unsigned int y, unsigned int size, const void* data)
	{
		// bind buffer
		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID));

		// make sure data will stay within the index size
		ASSERT(x < m_Layout->Size() && x >= 0);
		ASSERT(y < m_NumElements&& y >= 0);
		// write to buffer
		GLCall(glBufferSubData(GL_UNIFORM_BUFFER, m_Layout->ByteSize() * y + (*m_Layout)[x].offset, size, data));

		// make sure to tell OpenGL we're done with the pointer
		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
	}

	void UniformBuffer::GetBufferSubData(unsigned int x, unsigned int y, void* data)
	{
		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID));
		unsigned int size = BufferElement::GetSizeOfType((*m_Layout)[x].type);
		glGetBufferSubData(GL_UNIFORM_BUFFER, 0, m_Layout->ByteSize() * m_NumElements, data);
		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
	}

}