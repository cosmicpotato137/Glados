#include "TestTexture.h"

namespace test {

	using namespace Glados;

	TestTexture2D::TestTexture2D()
		: m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f)), 
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_Position(-50, -50, 0), blend(false)
	{
		float positions[]{
			0.0f,   0.0f,   0.0f, 0.0f,
			100.0f, 0.0f,   1.0f, 0.0f,
			100.0f, 100.0f, 1.0f, 1.0f,
			0.0f,   100.0f, 0.0f, 1.0f
		};

		unsigned int indices[]{
			0, 1, 2,
			0, 2, 3
		};

		// define blend function
		Renderer::SetBlend(blend);

		m_VAO = VertexArray::Create();

		BufferLayout layout({
			Uniform(UniformType::Float2, "position"),
			Uniform(UniformType::Float2, "texcoord")
			});

		m_VertexBuffer = VertexBuffer::Create(positions, layout.GetStride() * 4);
		m_VertexBuffer->SetLayout(layout);
		m_VAO->AddVertexBuffer(m_VertexBuffer);

		m_IndexBuffer = IndexBuffer::Create(&indices[0], 6);
		m_VAO->SetIndexBuffer(m_IndexBuffer);
		
		m_Shader = Renderer::GetShaderLibrary().Get("textureTest");

		m_Model = glm::mat4(1.0f);
		m_Model = glm::scale(m_Model, glm::vec3(3.0f, 3.0f, 3.0f));
		m_Model = glm::translate(m_Model, m_Position);

		m_Texture = Texture::Create("res/textures/dirt.png");
	}

	TestTexture2D::~TestTexture2D()
	{
	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{
		m_Shader->Bind();
		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_Shader->SetFloat4("u_Color", color);
		glm::mat4 mvp = m_Proj * m_View * m_Model;
		m_Shader->SetMat4("u_MVP", mvp);

		m_Texture->Bind();
		m_Shader->SetInt("u_Texture", 0);
		m_Texture->Unbind();
		m_Shader->Unbind();
	}

	void TestTexture2D::OnRender()
	{
		m_Shader->Bind();
		m_Texture->Bind();
		Renderer::DrawIndexed(m_VAO);
		m_Shader->Unbind();
		m_Texture->Unbind();
	}

	void TestTexture2D::OnImGuiRender()
	{
		if (ImGui::Checkbox("Enable Blending", &blend))
			Renderer::SetBlend(blend);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	void TestTexture2D::OnViewportResize(glm::vec2 viewportSize)
	{
		float aspect = viewportSize.x / viewportSize.y;
		m_Proj = glm::mat4(glm::ortho(-aspect * viewportSize.y, aspect * viewportSize.y, -viewportSize.y, viewportSize.y));
	}

	void TestTexture2D::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrollEvent>(BIND_EVENT_FN(TestTexture2D::OnMouseScrollEvent));
	}

	bool TestTexture2D::OnMouseScrollEvent(MouseScrollEvent& e)
	{
		m_Zoom += e.GetXOffset() * 5;
		return false;
	}
}