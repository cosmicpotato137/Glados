#pragma once

#include <unordered_map>
#include <string>

#include "Glados/Renderer/Shader.h"
#include "glm/glm.hpp"

namespace Glados {

	using ShaderSources = std::unordered_map<int, std::string>;
	using ShaderIDs = std::vector<int>;

	class OpenGLShader : public Shader
	{
	private:
		std::string m_Filepath;
		std::string m_Name;
		uint32_t m_RendererID;
		bool m_BuildStatus;
		UniformMap m_UniformCache;
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertex, const std::string& fragment);
		~OpenGLShader();

		virtual const std::string& GetName() const override { return m_Name; }
		virtual bool GetBuildStatus() const override { return m_BuildStatus; }
		virtual UniformMap GetUniforms() const override { return m_UniformCache; }

		virtual void BuildShader() override;
		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetUniforms() override;
		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetFloat3(const std::string& name, const float* value) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void SetFloat4(const std::string& name, const float* value) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;

		void SetUniformBlockIndex(const std::string& name, unsigned int binding);

	private:
		std::string ReadFile(const std::string& filepath);
		ShaderSources Preprocess(const std::string& source, const std::string& typetoken = "#shader");
		ShaderIDs Compile(ShaderSources shaderSources);
		int Link(ShaderIDs shaderIDs);
		UniformMap LoadUniforms();

		template<UniformType T> void SetUniform(Ref<Uniform> uniform);
	};
}