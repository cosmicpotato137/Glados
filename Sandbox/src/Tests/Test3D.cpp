#include "gladospch.h"
#include "Test3D.h"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/misc/cpp/imgui_stdlib.h"

namespace test {

	Test3D::Test3D()
		: m_Position(0, 0, -100), m_Rotation(45, 45, 45), m_Scale(20)
	{
		Renderer::SetBlend(true);
		//Renderer::SetDefaultShader(Shader::Create("res/shaders/basic.shader"));

		/// quad ///
		//std::array<float, 4 * 3> positions{
		//	1.0f,  -1.0f, 0.0f,
		//	-1.0f, -1.0f, 0.0f,
		//	0.0f,  1.0f,  1.0f,
		//	0.0f,  1.0f,  -1.0f
		//};
		
		std::array<float, 4 * 3> positions{
			0.0f, 0.0f, 0.0f,
			100.0f, 0.0f,	0.0f,
			100.0f, 100.0f,	0.0f,
			0.0f, 100.0f,	0.0f
		};

		std::array<uint32_t, 12> indices{
			0, 1, 2,
			0, 2, 3,
		};
		////////////

		// make model
		{
			m_VertexArray = VertexArray::Create();
			m_Mesh = Mesh::Create("res/models/Icosphere.obj", m_VertexArray);
		}

		m_View = glm::lookAt(vec3(0, 0, 100), vec3(0, 0, 0), vec3(0, 1, 0));

		ShaderLibrary& lib = Renderer::GetShaderLibrary();
		m_Shader = lib.Get("basic");
		m_Shader->Bind();
		m_Shader->SetFloat4("u_Color", glm::vec4(1, 1, 1, 0));

		// triangle
		//m_VertexArray = VertexArray::Create();
		//{
		//	BufferLayout layout({
		//		Uniform(UniformType::Float2, "position")
		//		});
		//	m_VertexBuffer = VertexBuffer::Create(&positions[0], layout.GetStride() * 4);
		//	m_VertexBuffer->SetLayout(layout);
		//}

		//m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		//m_IndexBuffer = IndexBuffer::Create(&indices[0], 6);
		//m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	}
	
	Test3D::~Test3D()
	{

	}

	void Test3D::OnUpdate(float timestep)
	{
		mat4 trans = translate(mat4(1.0f), m_Position);
		mat4 rotx = rotate(mat4(1.0f), radians(m_Rotation.x), vec3(0.0f, 1.0f, 0.0f));
		mat4 roty = rotate(mat4(1.0f), radians(m_Rotation.y), vec3(0.0f, 0.0f, 1.0f));
		mat4 rotz = rotate(mat4(1.0f), radians(m_Rotation.z), vec3(1.0f, 0.0f, 0.0f));
		mat4 scl = scale(mat4(1.0f), m_Scale);

		m_Transform = trans * rotx * roty * rotz * scl;
		
		
		m_Shader->Bind();
		m_Shader->SetMat4("u_ViewProjection", m_Projection);
		m_Shader->SetMat4("u_Transform", mat4(1));
		m_Shader->Unbind();
	}

	void Test3D::OnRender()
	{
		Renderer::Submit(m_Shader, m_VertexArray, m_Transform);
	}

	void Test3D::OnImGuiRender()
	{
		ImGui::DragFloat3("Position", &m_Position[0]);
		ImGui::DragFloat3("Rotation", &m_Rotation[0]);
		ImGui::DragFloat3("Scale", &m_Scale[0]);
		ImGui::DragFloat("Rotation Speed", &m_RotSpd);
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
		switch (e.GetKeyCode())
		{
		case GD_KEY_W: m_Rotation += m_RotSpd * vec3(0, 0, 1);  return true; 
		case GD_KEY_S: m_Rotation += m_RotSpd * vec3(0, 0, -1); return true;
		case GD_KEY_A: m_Rotation += m_RotSpd * vec3(1, 0, 0);  return true; 
		case GD_KEY_D: m_Rotation += m_RotSpd * vec3(-1, 0, 0); return true; 
		case GD_KEY_Q: m_Rotation += m_RotSpd * vec3(0, 1, 0);  return true; 
		case GD_KEY_E: m_Rotation += m_RotSpd * vec3(0, -1, 0); return true; 
		}

		return false;
	}

}