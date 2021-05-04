#include "gladospch.h"
#include "Renderer.h"

namespace Glados {

	Scope<RendererAPI> Renderer::s_RendererAPI = nullptr;
	Scope<ShaderLibrary> Renderer::s_ShaderLib = CreateScope<ShaderLibrary>();
	Ref<Shader> Renderer::s_DefaultShader = nullptr;
	Ref<Framebuffer> Renderer::s_Framebuffer = nullptr;

	void Renderer::Init()
	{
		s_RendererAPI = RendererAPI::Create();
		s_RendererAPI->Init();
		s_DefaultShader = Shader::Create("res/shaders/default.shader");
		s_Framebuffer = Framebuffer::Create(FramebufferSpecification());
	}

	void Renderer::Shutdown()
	{
		s_RendererAPI->Shutdown();
	}

	void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		s_RendererAPI->SetViewport(x, y, width, height);
		s_Framebuffer->Resize(width, height);
	}

	void Renderer::Clear()
	{
		s_Framebuffer->Bind();
		s_RendererAPI->Clear();
		s_Framebuffer->Unbind();
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
		s_Framebuffer->Bind();
		s_RendererAPI->DrawIndexed(vertexArray, count);
		s_Framebuffer->Unbind();
	}

}