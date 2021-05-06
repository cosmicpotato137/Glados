#include "gladospch.h"
#include "Renderer.h"
#include "Renderer2D.h"

namespace Glados {

	Scope<ShaderLibrary> Renderer::s_ShaderLib = CreateScope<ShaderLibrary>();
	Ref<Shader> Renderer::s_DefaultShader = nullptr;

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
		s_DefaultShader = Shader::Create("res/shaders/default.shader");
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		// TODO: implement scene data
		//shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		//shader->SetMat4("u_Transform", transform);
		RenderCommand::DrawIndexed(vertexArray);
		shader->Unbind();
	}

}