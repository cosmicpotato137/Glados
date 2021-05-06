#include "gladospch.h"
#include "Renderer.h"
#include "Material.h"
#include "imgui.h"

namespace Glados {

	static void UniformTypeImGuiWidget(const Ref<Uniform> u)
	{
		switch (u->Type)
		{
		case UniformType::Float:   ImGui::DragFloat(u->Name.c_str(), (float*)u->GetData(), 0.01f);	return;
		case UniformType::Float2:  ImGui::DragFloat2(u->Name.c_str(), (float*)u->GetData(), 0.01f); return;
		case UniformType::Float3:  ImGui::DragFloat3(u->Name.c_str(), (float*)u->GetData(), 0.01f); return;
		case UniformType::Float4:  ImGui::DragFloat4(u->Name.c_str(), (float*)u->GetData(), 0.01f); return;
		case UniformType::Int:     ImGui::DragInt(u->Name.c_str(), (int*)u->GetData());	return;
		case UniformType::Int2:    ImGui::DragInt2(u->Name.c_str(), (int*)u->GetData()); return;
		case UniformType::Int3:    ImGui::DragInt3(u->Name.c_str(), (int*)u->GetData()); return;
		case UniformType::Int4:    ImGui::DragInt4(u->Name.c_str(), (int*)u->GetData()); return;
		case UniformType::Bool:    ImGui::Checkbox(u->Name.c_str(), (bool*)u->GetData()); return;
		// TODO: fix this!
		//case UniformType::Mat3:
		//{
		//	glm::mat3 data = *(glm::mat3*)u->GetData();
		//	ImGui::Text(u->Name.c_str());
		//	ImGui::DragFloat3("a", &data[0][0], 0.01);
		//	ImGui::DragFloat3("b", &data[1][0], 0.01);
		//	ImGui::DragFloat3("b", &data[2][0], 0.01);
		//	return;
		//}
		//case UniformType::Mat4:
		//{
		//	glm::mat4 data = *(glm::mat4*)u->GetData();
		//	ImGui::Text(u->Name.c_str());
		//	ImGui::DragFloat4("a", &data[0][0], 0.01);
		//	ImGui::DragFloat4("b", &data[1][0], 0.01);
		//	ImGui::DragFloat4("c", &data[2][0], 0.01);
		//	ImGui::DragFloat4("d", &data[3][0], 0.01);
		//	ImGui::EndGroup();
		//	return;
		//}

		}
		ImGui::Text(u->Name.c_str());
		return;
	}

	Material::Material(const std::string& name, const Ref<Shader>& shader)
		: m_Shader(shader), m_ShaderName(shader->GetName())
	{
		OnUpdate();
	}

	Material::~Material()
	{
	}

	void Material::OnUpdate()
	{
	}

	void Material::OnImGuiRender()
	{
		/// select shader ///
		ImGui::SetNextItemWidth(200);
		ShaderLibrary shaderLib = Renderer::GetShaderLibrary();
		if (ImGui::BeginCombo("Shaders", m_ShaderName.c_str()))
		{
			for (auto shader : shaderLib)
			{
				bool is_selected = (m_ShaderName == shader.first);
				if (ImGui::Selectable(shader.first.c_str(), is_selected))
				{
					m_ShaderName = shader.first;
					m_Shader = Renderer::GetShaderLibrary().Get(m_ShaderName);
					m_Shader->BuildShader();
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		/////////////////////

		if (ImGui::Button("Recompile Shader"))
		{
			m_Shader->BuildShader();
		}

		if (!Renderer::GetShaderLibrary().Get(m_ShaderName)->GetBuildStatus())
		{
			ImGui::Text("Unable to build shader!");
			ImGui::Text("Using default.");
		}
		else
		{
			ImGui::Text("Uniforms");
			UniformMap shaderUniforms = m_Shader->GetUniforms();
			for (auto uniform : shaderUniforms)
			{
				UniformTypeImGuiWidget(uniform.second);
			}
		}
	}

	Ref<Material> Material::Create(const std::string& name, const Ref<Shader>& m_Shader)
	{
		return CreateRef<Material>(name, m_Shader);
	}

}