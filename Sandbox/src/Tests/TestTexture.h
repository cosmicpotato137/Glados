#pragma once

#include "Test.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
	
	class TestTexture2D : public Test
	{
	public:
		TestTexture2D();
		~TestTexture2D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void OnViewportResize(glm::vec2 viewportSize) override;
		void OnEvent(Event& e) override;

	private:
		bool OnMouseScrollEvent(MouseScrollEvent& e);
	private:
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<VertexArray> m_VAO;
		Ref<IndexBuffer> m_IndexBuffer;
		Ref<Shader> m_Shader;
		Ref<Texture2D> m_Texture;

		glm::mat4 m_Proj, m_View, m_Model;
		glm::vec3 m_Position;
		float m_Zoom;

		bool blend;
	};
}