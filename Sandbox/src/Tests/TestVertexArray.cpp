#include "TestVertexArray.h"
#include "glm/gtc/matrix_transform.hpp"

namespace test {

	TestVertexArray::TestVertexArray()
		: m_View(glm::lookAt(vec3(0,0,1),vec3(0,0,0), vec3(0, 1, 0))),
		m_Model(200, 200, 0), m_Color(1.0f, 0.5f, 1.0f, 1.0f), m_Camera(OrthographicCamera(0, 0, 0, 0))
	{
	}

	TestVertexArray::~TestVertexArray()
	{
	}

	void TestVertexArray::OnUpdate(float deltaTime)
	{
	}

	void TestVertexArray::OnRender()
	{
		Renderer2D::BeginScene(m_Camera);
		mat4 transf = scale(mat4(1), vec3(100));
		Renderer2D::DrawQuad(transf, m_Color);
		Renderer2D::DrawQuad(translate(mat4(1), vec3(105, 0, 0)) * transf, 1.0f / m_Color); 
		Renderer2D::EndScene();
	}

	void TestVertexArray::OnImGuiRender()
	{
		ImGui::ColorEdit4("Shader Color", &m_Color[0]);
	}

	bool TestVertexArray::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		m_Color = 1.0f / m_Color;
		return true;
	}

	void TestVertexArray::OnViewportResize(glm::vec2 viewportSize)
	{
		float aspect = viewportSize.x / viewportSize.y;
		m_Proj = glm::mat4(glm::ortho(-aspect * viewportSize.y, aspect * viewportSize.y, -viewportSize.y, viewportSize.y));
		m_Camera = OrthographicCamera(-aspect * viewportSize.y, aspect * viewportSize.y, -viewportSize.y, viewportSize.y);
	}

	void TestVertexArray::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(TestVertexArray::OnMouseButtonPressedEvent));
	}
}