#pragma once
#include "Glados/Renderer/VertexArray.h"

namespace Glados
{

	class OpenGLVertexArray : public VertexArray
	{
	private:
		uint32_t m_RendererID;
		Ref<IndexBuffer> m_IndexBuffer;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		uint32_t m_VertexBufferIndex = 0;
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vb) override;
		virtual std::vector<Ref<VertexBuffer>> GetVertexBuffers() const override { return m_VertexBuffers; }

		virtual void SetIndexBuffer(Ref<IndexBuffer>& ib) override;
		virtual IndexBuffer& GetIndexBuffer() const override { return *m_IndexBuffer; }

		virtual void Bind() const override;
		virtual void Unbind() const override;
	};

}
