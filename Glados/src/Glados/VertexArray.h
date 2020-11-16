#pragma once

#include "Core/Core.h"

#include "Buffer.h"
#include "BufferLayout.h"

namespace Glados {

	class GLADOS_API VertexArray
	{
	private:
		unsigned int m_RendererID;
	public:
		VertexArray();
		~VertexArray();

		void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout, unsigned int lpos = 0);

		void Bind() const;
		void Unbind() const;
	};

}