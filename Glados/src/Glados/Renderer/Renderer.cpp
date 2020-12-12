#include "gladospch.h"
#include "Renderer.h"

namespace Glados {

	Scope<RendererAPI> Renderer::s_RendererAPI = nullptr;
	Scope<ShaderLibrary> Renderer::s_ShaderLib = CreateScope<ShaderLibrary>();
	Ref<Shader> Renderer::s_DefaultShader = nullptr;

	void Renderer::Init()
	{
		s_RendererAPI = RendererAPI::Create();
		s_RendererAPI->Init();
		s_DefaultShader = Shader::Create("res/shaders/default.shader");
	}

	void Renderer::Shutdown()
	{
		s_RendererAPI->Shutdown();
	}

	void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		s_RendererAPI->SetViewport(x, y, width, height);
		
	}

	void Renderer::Clear()
	{
		s_RendererAPI->Clear();
	}

	void Renderer::SetClearColor(const vec4& color)
	{
		s_RendererAPI->SetClearColor(color);
	}

	void Renderer::SetBlend(bool blend)
	{
		s_RendererAPI->Blend(blend);
	}

	void Renderer::DrawIndexed(const Ref<VertexArray> vertexArray, uint32_t count)
	{
		s_RendererAPI->DrawIndexed(vertexArray, count);
	}

	void Renderer::SetDefaultShader(const Ref<Shader>& defaultShader)
	{
		s_DefaultShader = defaultShader;
	}

	Ref<Shader> Renderer::GetDefaultShader()
	{
		return s_DefaultShader;
	}

}