#include "gladospch.h"
#include "Test3D.h"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/misc/cpp/imgui_stdlib.h"

namespace test {

	Test3D::Test3D()
		: m_Position(0, 0, -100), m_Rotation(45, 45, 45), m_Scale(20)
	{
		Renderer::SetBlend(true);

		m_Shader = Renderer::GetShaderLibrary().Get("basic");
		m_Shader->Bind();
		m_Shader->SetFloat4("u_Color", glm::vec4(1));
		m_VertexArray = VertexArray::Create();
		m_Mesh = Mesh::Create("res/models/Icosphere.obj", m_VertexArray);
		m_View = glm::lookAt(glm::vec3(0, 0, -10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	}
	
	Test3D::~Test3D()
	{

	}

	void Test3D::OnUpdate(float timestep)
	{
		m_Shader->Bind();
		m_Shader->SetMat4("u_ViewProjection", m_Projection * m_View);
		m_Shader->SetMat4("u_Transform", glm::mat4(1));
		m_Shader->Unbind();
	}

	void Test3D::OnRender()
	{
		Renderer::Submit(m_Shader, m_VertexArray, mat4(1));
	}

	void Test3D::OnImGuiRender()
	{
	}

	void Test3D::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(Test3D::OnKeyPressedEvent));
	}

	void Test3D::OnViewportResize(glm::vec2 viewportSize)
	{
		float aspect = viewportSize.x / viewportSize.y;
		m_Projection = perspective(radians(90.0f), aspect, 0.1f, 1500.0f);
		//m_Projection = glm::mat4(glm::ortho(-aspect * viewportSize.y, aspect * viewportSize.y, -viewportSize.y, viewportSize.y));
	}

	bool Test3D::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		return false;
	}

}