#pragma once
#include "Shader.h"
#include "Uniform.h"

namespace Glados {

	//enum class UniformType
	//{
	//	None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	//};

	//static uint32_t UniformTypeSize(UniformType type)
	//{
	//	switch (type)
	//	{
	//	case UniformType::Float:    return 4;
	//	case UniformType::Float2:   return 4 * 2;
	//	case UniformType::Float3:   return 4 * 3;
	//	case UniformType::Float4:   return 4 * 4;
	//	case UniformType::Mat3:     return 4 * 3 * 3;
	//	case UniformType::Mat4:     return 4 * 4 * 4;
	//	case UniformType::Int:      return 4;
	//	case UniformType::Int2:     return 4 * 2;
	//	case UniformType::Int3:     return 4 * 3;
	//	case UniformType::Int4:     return 4 * 4;
	//	case UniformType::Bool:     return 1;
	//	}

	//	GD_CORE_ASSERT(false, "Unknown ShaderDataType!");
	//	return 0;
	//}

	//struct Uniform
	//{
	//	std::string Name;
	//	UniformType Type;
	//	uint32_t Size;
	//	size_t Offset;
	//	bool Normalized;

	//	Uniform() = default;

	//	Uniform(UniformType type, const std::string& name, bool normalized = false)
	//		: Name(name), Type(type), Size(UniformTypeSize(type)), Offset(0), Normalized(normalized)
	//	{
	//	}

	//	uint32_t GetComponentCount() const
	//	{
	//		switch (Type)
	//		{
	//		case UniformType::Float:   return 1;
	//		case UniformType::Float2:  return 2;
	//		case UniformType::Float3:  return 3;
	//		case UniformType::Float4:  return 4;
	//		case UniformType::Mat3:    return 3; // 3* float3
	//		case UniformType::Mat4:    return 4; // 4* float4
	//		case UniformType::Int:     return 1;
	//		case UniformType::Int2:    return 2;
	//		case UniformType::Int3:    return 3;
	//		case UniformType::Int4:    return 4;
	//		case UniformType::Bool:    return 1;
	//		}

	//		GD_CORE_ASSERT(false, "Unknown ShaderDataType!");
	//		return 0;
	//	}
	//};

	class BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(std::initializer_list<Uniform> elements)
			: m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		uint32_t GetStride() const { return m_Stride; }
		const std::vector<Uniform>& GetElements() const { return m_Elements; }

		std::vector<Uniform>::iterator begin() { return m_Elements.begin(); }
		std::vector<Uniform>::iterator end() { return m_Elements.end(); }
		std::vector<Uniform>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<Uniform>::const_iterator end() const { return m_Elements.end(); }
	private:
		void CalculateOffsetsAndStride()
		{
			size_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
	private:
		std::vector<Uniform> m_Elements;
		uint32_t m_Stride = 0;
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
	};


	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const void* data, uint32_t offset, uint32_t size) = 0;
		virtual float GetData(uint32_t offset, uint32_t size) const = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual BufferLayout GetLayout() const = 0;

		static Ref<VertexBuffer> Create(size_t size);
		static Ref<VertexBuffer> Create(float* vertices, size_t size);
	};


	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const void* data, uint32_t offset, size_t size) = 0;
		virtual float GetData(uint32_t offset, size_t size) const = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual BufferLayout GetLayout() const = 0;

		virtual void BindUniformBlock(const std::string& name, Ref<Shader> shader) = 0;

		static Ref<UniformBuffer> Create(size_t size);
		static Ref<UniformBuffer> Create(float* vertices, size_t size);
	};

}