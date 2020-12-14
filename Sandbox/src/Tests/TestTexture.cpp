#include "TestTexture.h"

namespace test {

	using namespace Glados;

	TestTexture2D::TestTexture2D()
		: m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f)), 
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_Model1(400, 200, 0), blend(false)
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
		m_Shader->Bind();
		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_Shader->SetFloat4("u_Color", color);

		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Model1);
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glm::mat4 mvp = m_Proj * m_View * model;
		m_Shader->SetMat4("u_MVP", mvp);

		m_Texture = Texture::Create("res/textures/dirt.png");
		m_Texture->Bind();
		m_Shader->SetInt("u_Texture", 0);
	}

	TestTexture2D::~TestTexture2D()
	{
	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{
	}

	void TestTexture2D::OnRender()
	{
		Renderer::DrawIndexed(m_VAO);
	}

	void TestTexture2D::OnImGuiRender()
	{
		if (ImGui::Checkbox("Enable Blending", &blend))
			Renderer::SetBlend(blend);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}