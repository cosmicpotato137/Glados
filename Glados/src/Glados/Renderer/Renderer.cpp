#include "gladospch.h"
#include "Renderer.h"
#include "Renderer2D.h"

namespace Glados {

	Scope<ShaderLibrary> Renderer::s_ShaderLib = CreateScope<ShaderLibrary>();
	Ref<Shader> Renderer::s_DefaultShader = nullptr;
	Ref<Framebuffer> Renderer::s_Framebuffer = nullptr;

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
		s_DefaultShader = Shader::Create("res/shaders/default.shader");
		s_Framebuffer = Framebuffer::Create(FramebufferSpecification());
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::OnWindowResize(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(x, y, width, height);
		s_Framebuffer->Resize(width, height);
	}

	void Renderer::Clear()
	{
		s_Framebuffer->Bind();
		RenderCommand::Clear();
		s_Framebuffer->Unbind();
	}

	void Renderer::SetClearColor(const vec4& color)
	{
		RenderCommand::SetClearColor(color);
	}

	void Renderer::SetBlend(bool blend)
	{
		RenderCommand::SetBlend(blend);
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		s_Framebuffer->Bind();
		shader->Bind();
		// TODO: implement scene data
		//shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		//shader->SetMat4("u_Transform", transform);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
		shader->Unbind();
		s_Framebuffer->Unbind();
	}

}