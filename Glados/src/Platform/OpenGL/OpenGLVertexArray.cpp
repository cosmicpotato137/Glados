#include "gladospch.h"

#include "OpenGLVertexArray.h"
#include "OpenGLBuffer.h"
#include "glad/glad.h"

namespace Glados {

	static GLenum ShaderTypeToGLType(UniformType type)
	{
		switch (type)
		{
		case UniformType::None:		return GL_NONE;
		case UniformType::Bool:		return GL_BOOL;
		case UniformType::Int:		return GL_INT;
		case UniformType::Int2:		return GL_INT;
		case UniformType::Int3:		return GL_INT;
		case UniformType::Int4:		return GL_INT;
		case UniformType::Float:		return GL_FLOAT;
		case UniformType::Float2:	return GL_FLOAT;
		case UniformType::Float3:	return GL_FLOAT;
		case UniformType::Float4:	return GL_FLOAT;
		case UniformType::Mat3:		return GL_FLOAT;
		case UniformType::Mat4:		return GL_FLOAT;
		}
		GD_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
		: m_VertexBufferCount(0)
	{
		glGenVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		//m_VertexBuffers.push_back(vertexBuffer);
		Bind();
		vertexBuffer->Bind();
		// list of all elements in m_Elements vector
		BufferLayout layout = vertexBuffer->GetLayout();
		for (Uniform e : layout)
		{
			GD_CORE_ASSERT(
				e.Type != UniformType::Mat4 && e.Type != UniformType::Mat3, 
				"Glados doesn't support Mat3 or Mat4 yet!"
			);

			// pushes data to the target buffer
			glEnableVertexAttribArray(m_VertexBufferCount);
			// formatting of data in current 
			glVertexAttribPointer(
				m_VertexBufferCount,
				e.GetComponentCount(),
				ShaderTypeToGLType(e.Type),
				e.Normalized,
				layout.GetStride(), 
				(const void*)e.Offset
			);
			m_VertexBufferCount++;
		}
	}

	void OpenGLVertexArray::SetIndexBuffer(Ref<IndexBuffer>& ib)
	{
		glBindVertexArray(m_RendererID);
		ib->Bind();
		m_IndexBuffer = ib;
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

}