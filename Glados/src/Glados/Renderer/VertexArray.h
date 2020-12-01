#pragma once
#include "Buffer.h"

namespace Glados {

	class VertexArray
	{
	public:
		~VertexArray() = default;

		virtual void AddBuffer(const VertexBuffer& vb, uint32_t lpos = 0) = 0;

		virtual void SetIndexBuffer(Ref<IndexBuffer>& ib) = 0;
		virtual IndexBuffer& GetIndexBuffer() const = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Ref<VertexArray> Create();
	};

}