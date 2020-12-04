#include "gladospch.h"
#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Glados {

	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (GD_GRAPHICS_API)
		{
		case GD_OPENGL_API:	return CreateRef<OpenGLShader>(filepath);
		}
		GD_CORE_ASSERT(false, "Unknown API!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexsrc, const std::string& fragmentsrc)
	{
		switch (GD_GRAPHICS_API)
		{
		case GD_OPENGL_API:	return CreateRef<OpenGLShader>(name, vertexsrc, fragmentsrc);
		}
		GD_CORE_ASSERT(false, "Unknown API!");
		return nullptr;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		if (!Exists(shader->GetName()))
			m_Shaders[shader->GetName()] = shader;
		else
			GD_CORE_WARN("Shader already exists!");
	}

	Glados::Ref<Glados::Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		Ref<Shader> shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Glados::Ref<Glados::Shader> ShaderLibrary::Get(const std::string& name)
	{
		GD_CORE_ASSERT(Exists(name), "Shader doesn't exist!");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name)
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}

}