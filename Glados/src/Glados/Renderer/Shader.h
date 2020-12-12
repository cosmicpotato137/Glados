#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include "Uniform.h"

namespace Glados {

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void BuildShader() = 0;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetUniforms() = 0;
		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat3(const std::string& name, const float* value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetFloat4(const std::string& name, const float* value) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

		virtual const std::string& GetName() const = 0;
		virtual bool GetBuildStatus() const = 0;
		virtual UniformMap GetUniforms() const = 0;

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
	};

	class ShaderLibrary
	{
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	public:
		void Add(const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& filepath);
		bool Exists(const std::string& name);

		Ref<Shader> Get(const std::string& name);

		std::unordered_map<std::string, Ref<Shader>>::iterator begin() { return m_Shaders.begin(); }
		std::unordered_map<std::string, Ref<Shader>>::iterator end() { return m_Shaders.end(); }
	};

}