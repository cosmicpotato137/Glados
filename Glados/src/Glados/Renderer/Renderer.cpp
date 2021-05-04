#include "gladospch.h"
#include "Renderer.h"

namespace Glados {

	Scope<ShaderLibrary> Renderer::s_ShaderLib = CreateScope<ShaderLibrary>();
	Ref<Shader> Renderer::s_DefaultShader = nullptr;
	Ref<Framebuffer> Renderer::s_Framebuffer = nullptr;

	void Renderer::Init()
	{
		RendererCommand::Init();
		s_DefaultShader = Shader::Create("res/shaders/default.shader");
		s_Framebuffer = Framebuffer::Create(FramebufferSpecification());
	}

	void Renderer::Shutdown()
	{
	}

	void Renderer::OnWindowResize(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		RendererCommand::SetViewport(x, y, width, height);
		s_Framebuffer->Resize(width, height);
	}

	void Renderer::Clear()
	{
		s_Framebuffer->Bind();
		RendererCommand::Clear();
		s_Framebuffer->Unbind();
	}

	void Renderer::SetClearColor(const vec4& color)
	{
		RendererCommand::SetClearColor(color);
	}

	void Renderer::SetBlend(bool blend)
	{
		RendererCommand::SetBlend(blend);
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		s_Framebuffer->Bind();
		shader->Bind();
		// TODO: implement scene data
		//shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		//shader->SetMat4("u_Transform", transform);
		vertexArray->Bind();
		RendererCommand::DrawIndexed(vertexArray);
		shader->Unbind();
		s_Framebuffer->Unbind();
	}

}