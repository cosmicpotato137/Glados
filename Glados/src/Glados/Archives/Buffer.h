#pragma once
#include "Core/Core.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "BufferLayout.h"

namespace Glados {

	class Buffer
	{
	protected:
		unsigned int m_RendererID;
		unsigned int m_NumElements;
		std::shared_ptr<BufferLayout> m_Layout;

	public:
		Buffer(const BufferLayout& layout, unsigned int numelts)
			: m_NumElements(numelts)
		{
			m_Layout = std::make_shared<BufferLayout>(layout);
			GLCall(glGenBuffers(1, &m_RendererID));
		}

		// pure virtual definition
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		inline unsigned int GetNumElements() const { return m_NumElements; }

		void GetBufferSubData(unsigned int offset, unsigned int size, void* data) const
		{
			GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID));
			glGetBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
			GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
		}

		void SetBufferSubData(unsigned int offset, unsigned int size, const void* data)
		{
			// bind buffer
			GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID));

			// write to buffer
			GLCall(glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data));

			// make sure to tell OpenGL we're done with the pointer
			GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
		}
	};

	class IndexBuffer : public Buffer
	{
	public:
		IndexBuffer(const IndexBufferLayout& layout, unsigned int numelts, const unsigned int* data, unsigned int usage = GL_STATIC_DRAW);
		~IndexBuffer();

		void Bind() const override;
		void Unbind() const override;
	};


	class VertexBuffer : public Buffer
	{
	public:
		VertexBuffer(const VertexBufferLayout& layout, unsigned int numelts, const void* data = NULL, unsigned int usage = GL_STATIC_DRAW);
		~VertexBuffer();

		void Bind() const override;
		void Unbind() const override;
	};


	class UniformBuffer : public Buffer
	{
	private:
		unsigned int m_Binding;
		//static std::unordered_map<unsigned int, unsigned int> m_UBB;

	public:
		UniformBuffer(const UniformBufferLayout& layout, unsigned int numelts, unsigned int binding, const void* data = NULL, unsigned int usage = GL_STATIC_DRAW);
		~UniformBuffer();

		void Bind() const override;
		void Unbind() const override;

		void BindUniformBlock(const std::string& name, std::shared_ptr<OpenGLShader> shader);
		void SetBufferSubData(unsigned int x, unsigned int y, unsigned int size, const void* data);
		void GetBufferSubData(unsigned int x, unsigned int y, void* data);
	};

}