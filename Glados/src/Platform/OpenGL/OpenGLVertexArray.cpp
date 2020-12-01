#include "gladospch.h"

#include "OpenGLVertexArray.h"
#include "OpenGLBuffer.h"
#include "glad/glad.h"

namespace Glados {

	static GLenum ShaderTypeToGLType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::None:		return GL_NONE;
		case ShaderDataType::Bool:		return GL_BOOL;
		case ShaderDataType::Int:		return GL_INT;
		case ShaderDataType::Int2:		return GL_INT;
		case ShaderDataType::Int3:		return GL_INT;
		case ShaderDataType::Int4:		return GL_INT;
		case ShaderDataType::Float:		return GL_FLOAT;
		case ShaderDataType::Float2:	return GL_FLOAT;
		case ShaderDataType::Float3:	return GL_FLOAT;
		case ShaderDataType::Float4:	return GL_FLOAT;
		case ShaderDataType::Mat3:		return GL_FLOAT;
		case ShaderDataType::Mat4:		return GL_FLOAT;
		}
		GD_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		GLCall(glGenVertexArrays(1, &m_RendererID));
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		GLCall(glDeleteVertexArrays(1, &m_RendererID));
	}

	void OpenGLVertexArray::AddBuffer(const VertexBuffer& vb, uint32_t layoutpos /*= 0*/)
	{
		Bind();
		vb.Bind();
		// list of all elements in m_Elements vector
		BufferLayout layout = vb.GetLayout();
		int vaa = layoutpos;
		for (BufferElement e : layout)
		{
			// pushes data to the target buffer
			GLCall(glEnableVertexAttribArray(vaa));
			// formatting of data in current 
			GLCall(glVertexAttribPointer(vaa, e.GetComponentCount(), ShaderTypeToGLType(e.Type),
				e.Normalized, layout.GetStride(), (const void*)e.Offset));

			vaa++;
		}
	}

	void OpenGLVertexArray::SetIndexBuffer(Ref<IndexBuffer>& ib)
	{
		m_IndexBuffer = ib;
	}

	IndexBuffer& OpenGLVertexArray::GetIndexBuffer() const
	{
		return *m_IndexBuffer;
	}

	void OpenGLVertexArray::Bind() const
	{
		GLCall(glBindVertexArray(m_RendererID));
	}

	void OpenGLVertexArray::Unbind() const
	{
		GLCall(glBindVertexArray(0));
	}

}