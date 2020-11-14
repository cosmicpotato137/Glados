#pragma once
#include "Core/Core.h"
#include <vector>

#include <GL/glew.h>
#include "core/GDLog.h"
#include "glm/glm.hpp"

namespace Glados {

	struct GLADOS_API BufferElement
	{
		unsigned int type;
		unsigned int count;
		unsigned int offset;
		unsigned char normalized;

		static unsigned int GetSizeOfType(unsigned int type)
		{
			switch (type)
			{
			case GL_FLOAT:			return sizeof(GLfloat);
			case GL_UNSIGNED_INT:	return sizeof(GLuint);
			case GL_UNSIGNED_BYTE:	return sizeof(GLbyte);
			case GL_FLOAT_VEC3:		return sizeof(glm::vec3);
			case GL_FLOAT_VEC4:		return sizeof(glm::vec4);
			case GL_FLOAT_MAT4:		return sizeof(glm::mat4);
			}
			ASSERT(false);
			return 0;
		}
	};

	class GLADOS_API BufferLayout
	{
	protected:
		std::vector<BufferElement> m_Elements;
		unsigned int m_ByteSize;

	public:
		BufferLayout() : m_ByteSize(0) {}

		BufferElement operator[](unsigned int i) { return m_Elements[i]; }

		inline const std::vector<BufferElement> GetElements() const { return m_Elements; }
		inline unsigned int ByteSize() const { return m_ByteSize; }
		inline unsigned int Size() const { return m_Elements.size(); }
	};

	class GLADOS_API VertexBufferLayout : public BufferLayout
	{
	public:
		VertexBufferLayout() : BufferLayout() {}

		template <typename T>
		void Push(unsigned int count) { static_assert(false); }

		template<>
		void Push<float>(unsigned int count)
		{
			m_Elements.push_back({ GL_FLOAT, count, m_ByteSize, GL_FALSE });
			m_ByteSize += count * BufferElement::GetSizeOfType(GL_FLOAT);
		}

		template<>
		void Push<glm::vec3>(unsigned int count)
		{
			for (unsigned int i = 0; i < count; i++)
				Push<float>(3);
		}

		template<>
		void Push<glm::vec4>(unsigned int count)
		{
			for (unsigned int i = 0; i < count; i++)
				Push<float>(4);
		}

		template<>
		void Push<unsigned int>(unsigned int count)
		{
			m_Elements.push_back({ GL_UNSIGNED_INT, count, m_ByteSize, GL_FALSE });
			m_ByteSize += count * BufferElement::GetSizeOfType(GL_UNSIGNED_INT);
		}

		template<>
		void Push<char>(unsigned int count)
		{
			m_Elements.push_back({ GL_UNSIGNED_BYTE, count, m_ByteSize, GL_TRUE });
			m_ByteSize += count * BufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
		}

	};

	class GLADOS_API IndexBufferLayout : public BufferLayout
	{
	public:
		IndexBufferLayout() : BufferLayout() {};

		void Push(unsigned int count)
		{
			m_Elements.push_back({ GL_UNSIGNED_INT, count, m_ByteSize, GL_FALSE });
			m_ByteSize += count * BufferElement::GetSizeOfType(GL_UNSIGNED_INT);
		}
	};

	class GLADOS_API UniformBufferLayout : public BufferLayout
	{
	public:
		UniformBufferLayout() : BufferLayout() {};

		template <typename T>
		void Push(unsigned int count) { static_assert(false); }

		template<>
		void Push<float>(unsigned int count)
		{
			m_Elements.push_back({ GL_FLOAT, count, m_ByteSize, GL_FALSE });
			m_ByteSize += count * BufferElement::GetSizeOfType(GL_FLOAT_VEC4);
		}

		template<>
		void Push<glm::vec3>(unsigned int count)
		{
			m_Elements.push_back({ GL_FLOAT_VEC3, count, m_ByteSize, GL_FALSE });
			m_ByteSize += count * BufferElement::GetSizeOfType(GL_FLOAT_VEC4);
		}

		template<>
		void Push<glm::vec4>(unsigned int count)
		{
			m_Elements.push_back({ GL_FLOAT_VEC4, count, m_ByteSize, GL_FALSE });
			m_ByteSize += count * BufferElement::GetSizeOfType(GL_FLOAT_VEC4);
		}

		template<>
		void Push<unsigned int>(unsigned int count)
		{
			m_Elements.push_back({ GL_UNSIGNED_INT, count, m_ByteSize, GL_FALSE });
			m_ByteSize += count * BufferElement::GetSizeOfType(GL_FLOAT_VEC4);
		}

		template<>
		void Push<char>(unsigned int count)
		{
			m_Elements.push_back({ GL_UNSIGNED_BYTE, count, m_ByteSize, GL_TRUE });
			m_ByteSize += count * BufferElement::GetSizeOfType(GL_FLOAT_VEC4);
		}
	};
}