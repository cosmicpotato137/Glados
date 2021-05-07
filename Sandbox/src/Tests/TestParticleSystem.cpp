#include "gladospch.h"
#include "TestParticleSystem.h"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/misc/cpp/imgui_stdlib.h"

namespace test {

	TestParticleSystem::TestParticleSystem()
	{
		RenderCommand::SetBlend(true);
		RenderCommand::SetClearColor(vec4(0, .3, .3, 1));
	}

	TestParticleSystem::~TestParticleSystem()
	{
	}

	void TestParticleSystem::OnUpdate(float timestep)
	{
		m_Camera.OnUpdate(timestep);
	}

	void TestParticleSystem::OnRender()
	{
		Renderer2D::BeginScene(m_Camera);
		Renderer2D::DrawQuad(scale(mat4(1), vec3(1)), vec4(0, 1, .8, 1));
		Renderer2D::EndScene();
	}

	void TestParticleSystem::OnImGuiRender()
	{
	}

	void TestParticleSystem::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		m_Camera.OnEvent(e);
	}

	void TestParticleSystem::OnViewportResize(glm::vec2 viewportSize)
	{
		m_Camera.SetViewportSize(viewportSize.x, viewportSize.y);
	}

}