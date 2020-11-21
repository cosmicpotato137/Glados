#pragma once

#include "Test.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Glados/Object.h"

namespace test {

	class TestUniformBuffer : public Test
	{
	public:
		TestUniformBuffer();
		~TestUniformBuffer();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::shared_ptr<Glados::Material> m_Mat1;
		std::unique_ptr<Glados::Object> model;
		std::shared_ptr<Glados::VertexArray> m_VAO;
		std::shared_ptr<glm::mat4> m_Proj, m_View;

		std::vector<std::shared_ptr<Glados::Object>>* m_Lights;
		std::shared_ptr<Glados::UniformBuffer> m_LightBuffer;

	};
}