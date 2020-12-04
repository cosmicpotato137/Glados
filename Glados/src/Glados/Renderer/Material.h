# pragma once
#include "Core/Core.h"
#include <string>

#include "ObjAttrib.h"
#include "Renderer/Shader.h"

namespace Glados {

	class Material : public ObjAttrib
	{
	public:
		std::string name;
		std::shared_ptr<Shader> shader;
		//std::unordered_map<std::string, std::unique_ptr<ShaderAttrib>> shaderAttribs;
		std::string shaderPath;
		std::string materialPath;

		glm::vec4 diffuseCol;
		glm::vec4 ambientCol;
		float specInt;
		glm::vec4 specCol;

		std::shared_ptr<UniformBuffer> lightBuffer;

	public:
		Material(const std::string& name, const std::string& shaderpath, const std::string& matpath = NULL, std::shared_ptr<UniformBuffer> lightbuffer = NULL);
		~Material();

		void OnUpdate() override;
		void OnImGuiRender() override;

	private:
		void Parse(const std::string& matfile);
	};

}