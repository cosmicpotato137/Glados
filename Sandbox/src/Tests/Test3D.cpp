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
			1.0f, 0.0f,	0.0f,
			1.0f, 1.0f,	0.0f,
			0.0f, 1.0f,	0.0f
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

		Window& window = Application::Get().GetWindow();
		float w = (float)window.GetWidth();
		float h = (float)window.GetHeight();
		float aspect = w / h;
		m_Projection = perspective(radians(90.0f), aspect, 0.1f, 1500.0f);
		//m_Projection = ortho(0.0f, (float)window.GetWidth(), 0.0f, (float)window.GetHeight());
		m_View = lookAt(vec3(0.0f, 0.0f, 100.0f), vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));

		ShaderLibrary& lib = Renderer::GetShaderLibrary();
		Ref<Shader> shader = lib.Get("basic");
		m_Material = Material::Create("Material", shader);
	}
	
	Test3D::~Test3D()
	{

	}

	void Test3D::OnUpdate(float timestep)
	{
		m_Model = mat4(1);
		m_Model = m_Model * translate(mat4(1.0f), m_Position);
		m_Model = m_Model * scale(mat4(1.0f), m_Scale);
		m_Model = m_Model * rotate(mat4(1.0f), radians(m_Rotation.x), vec3(0.0f, 1.0f, 0.0f));
		m_Model = m_Model * rotate(mat4(1.0f), radians(m_Rotation.y), vec3(0.0f, 0.0f, 1.0f));
		m_Model = m_Model * rotate(mat4(1.0f), radians(m_Rotation.z), vec3(1.0f, 0.0f, 0.0f));
		
		m_Material->OnUpdate();
		
		Ref<Shader> shader = m_Material->GetShader();

		// TODO: abstract/clean up further
		mat4 modelView = m_View * m_Model;
		UniformMap um = shader->GetUniforms();
		um.GetUniform("u_ModelView")->SetData(&modelView[0][0]);
		um.GetUniform("u_Projection")->SetData(&m_Projection[0][0]);

		shader->Bind();
		shader->SetUniforms();
		shader->Unbind();

	}

	void Test3D::OnRender()
	{
		m_Material->GetShader()->Bind();
		Renderer::DrawIndexed(m_VertexArray);
		m_Material->GetShader()->Unbind();
	}

	void Test3D::OnImGuiRender()
	{
		ImGui::DragFloat3("Position", &m_Position[0]);
		ImGui::DragFloat3("Rotation", &m_Rotation[0]);
		ImGui::DragFloat3("Scale", &m_Scale[0]);

		m_Material->OnImGuiRender();
	}

	void Test3D::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Test3D::OnWindowResizeEvent));
	}

	bool Test3D::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		Window& window = Application::Get().GetWindow();
		float aspect = (float)window.GetWidth() / (float)window.GetHeight();
		m_Projection = perspective(radians(90.0f), aspect, 0.1f, 1500.0f);

		return false;
	}

}