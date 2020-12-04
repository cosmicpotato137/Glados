#pragma once

#include <unordered_map>
#include <string>

#include "Glados/Renderer/Shader.h"
#include "glm/glm.hpp"

namespace Glados {

	struct ShaderUniform
	{
		unsigned int binding;
		unsigned int type;
		const void* data;

		bool operator==(unsigned int b)
		{
			return b == binding;
		}
	};

	struct ShaderSource
	{
		std::string Source;
		uint32_t ID;
	};

	class OpenGLShader : public Shader
	{
	private:
		std::string m_Filepath;
		std::string m_Name;
		uint32_t m_RendererID;
		std::unordered_map<uint32_t, ShaderSource> m_ShaderSources;
		std::unordered_map<std::string, int> m_UniformLocationCache;
	public:
		OpenGLShader(const std::string& name, const std::string& filepath);
		~OpenGLShader();

		virtual const std::string& GetName() const override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;

		void SetUniformBlockIndex(const std::string& name, unsigned int binding);

		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void SetUniform4fv(const std::string& name, const float* value);
		void SetUniform3f(const std::string& name, float v0, float v1, float v2);
		void SetUniform3fv(const std::string& name, const float* value);
		void SetUniformMat4fv(const std::string& name, bool transpose, const float* value);
		void SetUniform1f(const std::string& name, float f);
		void SetUniform1i(const std::string& name, int i);
		void SetUniformMat4f(const std::string& name, glm::mat4& matrix);

		void PrintUniforms();
	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<uint32_t, ShaderSource> Preprocess(const std::string& shadersource, const std::string& typetoken = "#type");
		void Compile();
		uint32_t Link();

		int GetUniformLocation(const std::string& name);
	};

}