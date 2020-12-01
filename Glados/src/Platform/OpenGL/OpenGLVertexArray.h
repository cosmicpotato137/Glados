#pragma once
#include "Glados/Renderer/VertexArray.h"

namespace Glados
{

	class OpenGLVertexArray : public VertexArray
	{
	private:
		uint32_t m_RendererID;
		Ref<IndexBuffer> m_IndexBuffer;
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		virtual void AddBuffer(const VertexBuffer& vb, uint32_t layoutpos = 0) override;

		virtual void SetIndexBuffer(Ref<IndexBuffer>& ib) override;
		virtual IndexBuffer& GetIndexBuffer() const override;

		virtual void Bind() const override;
		virtual void Unbind() const override;
	};

}
