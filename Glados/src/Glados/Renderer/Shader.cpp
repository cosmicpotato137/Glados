#include "gladospch.h"

#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Glados {

	Ref<Shader> Shader::Create(const std::string& name, std::string& filepath)
	{
		return CreateRef<OpenGLShader>(name, filepath);
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexsrc, const std::string& fragmentsrc)
	{
		return Ref<Shader>();
	}

}