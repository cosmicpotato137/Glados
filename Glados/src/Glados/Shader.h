#pragma once

#include "Core/Core.h"
#include <unordered_map>
#include <string>

#include "glm/glm.hpp"

namespace Glados {

	class Object;

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

	struct ShaderProgramSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	class Shader
	{
	private:
		std::string m_Filepath;
		//std::unordered_map<std::string, std::unique_ptr<ShaderAttrib>> shaderAttribs;

		std::unordered_map<std::string, int> m_UniformLocationCache;
	public:
		unsigned int m_RendererID;
		Shader(const std::string& filepath);
		~Shader();

		void Bind() const;
		void Unbind() const;

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
		ShaderProgramSource ParseShader(const std::string& filepath);
		unsigned int CompileShader(unsigned int type, const std::string& source);
		int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
		int GetUniformLocation(const std::string& name);
	};

}