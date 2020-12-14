#pragma once
#include <unordered_map>
#include "glm/glm.hpp"

namespace Glados {

	enum class UniformType
	{
		None = 0, 
		Float, Float2, Float3, Float4, 
		Mat3, Mat4, 
		Int, Int2, Int3, Int4, Bool,
		Sampler1, Sampler2
	};

	static uint32_t UniformTypeSize(UniformType attrib);

	struct Uniform
	{
		std::string Name;
		UniformType Type;
		size_t Size;
		uint32_t Offset;
		bool Normalized;
	private:
		uint32_t m_ID;
		void* m_Data;
	public:
		Uniform();
		Uniform(UniformType type, const std::string& name, bool normalized = false);

		void SetID(uint32_t id);
		void SetData(void* data);
		uint32_t GetID() { return m_ID; }
		void* GetData();

		uint32_t GetComponentCount() const;
	};

	struct UniformMap
	{
	private:
		std::unordered_map<std::string, Ref<Uniform>> m_UniformMap;
	public:
		UniformMap();

		void AddUniform(const Ref<Uniform>& u);
		void Reset();
		bool Exists(const std::string& uniformName);
		uint32_t Size() { return m_UniformMap.size(); }
		const Ref<Uniform>& GetUniform(const std::string& uniformName);

		std::unordered_map<std::string, Ref<Uniform>>::iterator begin() { return m_UniformMap.begin(); }
		std::unordered_map<std::string, Ref<Uniform>>::iterator end() { return m_UniformMap.end(); }
	};

}