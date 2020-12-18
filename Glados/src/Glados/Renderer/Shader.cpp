#include "gladospch.h"
#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Glados/Renderer/Renderer.h"

namespace Glados {

	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetRenderAPI())
		{
		case RendererAPI::API::None:	GD_CORE_ASSERT(false, "RendererAPI::API::None is not currently supported!");
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLShader>(filepath);
		}
		GD_CORE_ASSERT(false, "Unknown RendererAPI::API!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexsrc, const std::string& fragmentsrc)
	{
		switch (Renderer::GetRenderAPI())
		{
		case RendererAPI::API::None:	GD_CORE_ASSERT(false, "RendererAPI::API::None is not currently supported!");
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLShader>(name, vertexsrc, fragmentsrc);
		}
		GD_CORE_ASSERT(false, "Unknown RendererAPI::API!");
		return nullptr;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		GD_CORE_VALIDATE(!Exists(shader->GetName()), return, "Shader: {0} already exists!", shader->GetName());
		m_Shaders[shader->GetName()] = shader;
	}

	Glados::Ref<Glados::Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		Ref<Shader> shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	bool ShaderLibrary::Exists(const std::string& name)
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		GD_CORE_VALIDATE(Exists(name), return Renderer::GetDefaultShader(), "Shader: {0} doesn't exist!", name);
		return m_Shaders[name];
	}
}