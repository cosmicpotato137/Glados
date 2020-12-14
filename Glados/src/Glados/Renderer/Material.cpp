#include "gladospch.h"
#include "Renderer.h"
#include "Material.h"
#include "imgui.h"

namespace Glados {

	static void UniformTypeImGuiWidget(const Ref<Uniform> u)
	{
		switch (u->Type)
		{
		case UniformType::Float:   ImGui::DragFloat(u->Name.c_str(), (float*)u->GetData(), 0.01);	 return;
		case UniformType::Float2:  ImGui::DragFloat2(u->Name.c_str(), (float*)u->GetData(), 0.01); return;
		case UniformType::Float3:  ImGui::DragFloat3(u->Name.c_str(), (float*)u->GetData(), 0.01); return;
		case UniformType::Float4:  ImGui::DragFloat4(u->Name.c_str(), (float*)u->GetData(), 0.01); return;
		case UniformType::Int:     ImGui::DragInt(u->Name.c_str(), (int*)u->GetData());	return;
		case UniformType::Int2:    ImGui::DragInt2(u->Name.c_str(), (int*)u->GetData()); return;
		case UniformType::Int3:    ImGui::DragInt3(u->Name.c_str(), (int*)u->GetData()); return;
		case UniformType::Int4:    ImGui::DragInt4(u->Name.c_str(), (int*)u->GetData()); return;
		case UniformType::Bool:    ImGui::Checkbox(u->Name.c_str(), (bool*)u->GetData()); return;
		}
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