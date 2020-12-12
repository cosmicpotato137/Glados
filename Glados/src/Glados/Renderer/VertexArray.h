#pragma once
#include "Buffer.h"

namespace Glados {

	class VertexArray
	{
	public:
		~VertexArray() = default;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vb) = 0;
		virtual std::vector<Ref<VertexBuffer>> GetVertexBuffers() const = 0;

		virtual void SetIndexBuffer(Ref<IndexBuffer>& ib) = 0;
		virtual IndexBuffer& GetIndexBuffer() const = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Ref<VertexArray> Create();
	};

}