#include "TestVertexArray.h"
#include "glm/gtc/matrix_transform.hpp"

namespace test {

	TestVertexArray::TestVertexArray()
		: m_Proj(ortho(0.0f, 960.0f, 0.0f, 540.0f)),
		m_View(glm::translate(mat4(1.0f), vec3(0, 0, 0))),
		m_Model(200, 200, 0), m_Camera(0, 0, 0), m_Color(1.0f, 0.5f, 1.0f, 1.0f)
	{
		float positions[]{
			0.0f,   0.0f,
			100.0f, 0.0f,
			100.0f, 100.0f,
			0.0f,   100.0f,
		};

		unsigned int indices[]{
			0, 1, 2,
			0, 2, 3
		};

		Renderer::SetClearColor(vec4(0.8, 0.2, 0.8, 1));

		m_VAO = VertexArray::Create();

		{
			BufferLayout layout({
				BufferElement(ShaderDataType::Float2, "position")
				});
			m_VertexBuffer = VertexBuffer::Create(&positions[0], layout.GetStride() * 4);
			m_VertexBuffer->SetLayout(layout);
		}

		m_VAO->AddBuffer(*m_VertexBuffer);

		m_IndexBuffer = IndexBuffer::Create(&indices[0], 6);
		m_VAO->SetIndexBuffer(m_IndexBuffer);

		std::string name = "Basic";
		std::string filepath = "res/shaders/basic.shader";

		m_Shader = Renderer::GetShaderLibrary().Load(filepath);
		m_Shader->Bind();
		m_Shader->SetMat4("u_MVP", m_Proj * m_View);
	}

	TestVertexArray::~TestVertexArray()
	{

	}

	void TestVertexArray::OnUpdate(float deltaTime)
	{
		m_Shader->SetFloat4("u_Color", m_Color);
	}

	void TestVertexArray::OnRender()
	{
		m_Shader->Bind();
		Renderer::DrawIndexed(m_VAO);
	}

	void TestVertexArray::OnImGuiRender()
	{
		ImGui::ColorEdit4("Shader Color", &m_Color[0]);
	}

	void TestVertexArray::OnEvent(Event& e)
	{

	}

	bool TestVertexArray::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		m_Color = 1.0f / m_Color;
		return true;
	}

}