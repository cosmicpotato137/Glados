#include "gladospch.h"

#include "Material.h"
#include "imgui.h"

namespace Glados {

	Material::Material(const std::string& name, const std::string& shaderpath, const std::string& matpath, std::shared_ptr<UniformBuffer> lightbuffer)
		: name(name), shaderPath(shaderpath), materialPath(matpath), lightBuffer(lightbuffer)
	{
		if (&matpath != NULL)
			Parse(matpath);
		else
		{
		}

		shader = std::make_shared<Shader>(shaderpath);

		// bind the light buffer to LightBlock in shader
		if (&lightbuffer != NULL)
			lightBuffer->BindUniformBlock("LightBlock", shader);

		shader->PrintUniforms();
		OnUpdate();
	}

	Material::~Material()
	{
	}

	void Material::OnUpdate()
	{
		shader->Bind();
		shader->SetUniform4fv("u_Ambient", &ambientCol[0]);
		shader->SetUniform4fv("u_Diffuse", &diffuseCol[0]);
		shader->SetUniform4fv("u_Specular", &specCol[0]);
		shader->SetUniform1f("u_SpecInt", specInt);
	}

	void Material::OnImGuiRender()
	{
		if (!ImGui::TreeNode("Material"))
			return;
		ImGui::Text(shaderPath.c_str());
		ImGui::ColorEdit4("Diffuse Color", &diffuseCol[0]);
		ImGui::ColorEdit4("Ambient Color", &ambientCol[0]);
		ImGui::ColorEdit4("Specular Color", &specCol[0]);
		ImGui::SliderFloat("Specular Intensity", &specInt, 0.0f, 500.0f);
		ImGui::TreePop();
	}

	void Material::Parse(const std::string& matfile)
	{
		FILE* fp;
		float hi;
		float va, vb, vc;
		char c1, c2;

		fopen_s(&fp, matfile.c_str(), "rb");

		if (fp == NULL) {
			std::cerr << "Error loading file: " << matfile << std::endl;
			std::getchar();
			exit(-1);
		}

		// feof returns end of file
		while (!feof(fp)) {
			c1 = fgetc(fp); // returns character as int
			while (!(c1 == 'K' || c1 == 'N' || c1 == 'd' || c1 == 'i')) {
				c1 = fgetc(fp);
				if (feof(fp))
					break;
			}
			c2 = fgetc(fp);

			// get vertices
			if ((c1 == 'N') && (c2 == 's')) {
				fscanf_s(fp, " %f", &hi);
				specInt = hi;
			}
			// get normals
			else if ((c1 == 'K') && (c2 == 'a')) {
				fscanf_s(fp, " %f %f %f", &va, &vb, &vc);
				ambientCol = glm::vec4(va, vb, vc, 1.0f);
			}
			else if (c1 == 'K' && c2 == 'd')
			{
				fscanf_s(fp, " %f %f %f", &va, &vb, &vc);
				diffuseCol = glm::vec4(va, vb, vc, 1.0f);
			}
			else if (c1 == 'K' && c2 == 's')
			{
				fscanf_s(fp, " %f %f %f", &va, &vb, &vc);
				specCol = glm::vec4(va, vb, vc, 1.0f);
			}
		}
	}

}