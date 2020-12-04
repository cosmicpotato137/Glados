#pragma once

#include "Test.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Glados/Renderer/Buffer.h"
#include "Glados/Renderer/VertexArray.h"
#include "Glados/Renderer/Texture.h"
#include "Glados/Renderer/Shader.h"

namespace test {
	
	class TestTexture2D : public Test
	{
	public:
		TestTexture2D();
		~TestTexture2D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<VertexArray> m_VAO;
		Ref<IndexBuffer> m_IndexBuffer;
		Ref<Shader> m_Shader;
		Ref<Texture> m_Texture;

		glm::mat4 m_Proj, m_View;
		glm::vec3 m_Model1;

		bool blend;
	};
}