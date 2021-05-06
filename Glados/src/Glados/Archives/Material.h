# pragma once
#include <string>
#include "Glados/Core/Core.h"
#include "Shader.h"

namespace Glados {

	class Material
	{
	private:
		std::string m_Name;
		Ref<Shader> m_Shader;
		std::string m_ShaderName;
	public:
		Material(const std::string& name, const Ref<Shader>& m_Shader);
		~Material();

		void OnUpdate();
		void OnImGuiRender();

		std::string GetName() const { return m_Name; }
		Ref<Shader> GetShader() const { return m_Shader->GetBuildStatus() ? m_Shader : Renderer::GetDefaultShader(); }

		static Ref<Material> Create(const std::string& name, const Ref<Shader>& m_Shader);
	};

}