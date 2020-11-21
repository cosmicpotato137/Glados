#pragma once

#include "Test.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Glados/Buffer.h"
#include "Glados/VertexArray.h"
#include "Glados/Shader.h"

namespace test {
	
	class TestTransform2D : public Test
	{
	public:
		TestTransform2D();
		~TestTransform2D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::unique_ptr<Glados::VertexBuffer> m_VertexBuffer;
		std::unique_ptr<Glados::VertexArray> m_VAO;
		std::unique_ptr<Glados::IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Glados::Shader> m_Shader;

		glm::mat4 m_Proj, m_View;
		glm::vec3 m_Model, m_Camera;
	};
}