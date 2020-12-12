#include "gladospch.h"
#include "Uniform.h"

namespace Glados {

	uint32_t UniformTypeSize(UniformType attrib)
	{
		switch (attrib)
		{
		case UniformType::Float:    return 4;
		case UniformType::Float2:   return 4 * 2;
		case UniformType::Float3:   return 4 * 3;
		case UniformType::Float4:   return 4 * 4;
		case UniformType::Mat3:     return 4 * 3 * 3;
		case UniformType::Mat4:     return 4 * 4 * 4;
		case UniformType::Int:      return 4;
		case UniformType::Int2:     return 4 * 2;
		case UniformType::Int3:     return 4 * 3;
		case UniformType::Int4:     return 4 * 4;
		case UniformType::Bool:     return 1;
		}
		return 0;
	}

	Uniform::Uniform(UniformType type, const std::string& name, bool normalized) 
		: Name(name), Type(type), Size(UniformTypeSize(type)), Normalized(normalized)
	{
		m_Data = malloc(Size);
	}

	void* Uniform::GetData()
	{
		GD_CORE_ASSERT(m_Data != nullptr, "Uniform data uninitialized!");
		return m_Data;
	}

	void Uniform::SetData(void* data)
	{
		GD_CORE_ASSERT(data, "Data pointer is null!");
		m_Data = data;
	}

	void Uniform::SetID(uint32_t id)
	{
		m_ID = id;
	}

	uint32_t Uniform::GetComponentCount() const
	{
		switch (Type)
		{
		case UniformType::Float:   return 1;
		case UniformType::Float2:  return 2;
		case UniformType::Float3:  return 3;
		case UniformType::Float4:  return 4;
		case UniformType::Mat3:    return 3; // 3* float3
		case UniformType::Mat4:    return 4; // 4* float4
		case UniformType::Int:     return 1;
		case UniformType::Int2:    return 2;
		case UniformType::Int3:    return 3;
		case UniformType::Int4:    return 4;
		case UniformType::Bool:    return 1;
		}

		GD_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	void UniformMap::AddUniform(const Ref<Uniform>& u)
	{
		GD_CORE_VALIDATE(!Exists(u->Name), return, "Can not have duplicate uniforms!");
		m_UniformMap[u->Name] = u;
	}

	void UniformMap::Reset()
	{
		if (&m_UniformMap && m_UniformMap.size() > 0)
			m_UniformMap.clear();
	}

	const Glados::Ref<Glados::Uniform>& UniformMap::GetUniform(const std::string& uniformName)
	{
		GD_CORE_ASSERT(Exists(uniformName), "Uniform doesn't exist!");
		return m_UniformMap[uniformName];
	}

	bool UniformMap::Exists(const std::string& uniformName)
	{
		return m_UniformMap.find(uniformName) != m_UniformMap.end();
	}

}