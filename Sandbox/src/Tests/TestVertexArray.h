#pragma once
#include "Test.h"
#include "glm/glm.hpp"

using namespace glm;

namespace test {

	class TestVertexArray : public Test
	{
	private:
		mat4 m_Proj;
		mat4 m_View;
		vec3 m_Model;
		vec3 m_Camera;
		vec4 m_Color;

		Ref<VertexArray>	m_VAO;
		Ref<VertexBuffer>	m_VertexBuffer;
		Ref<IndexBuffer>	m_IndexBuffer;
		Ref<Shader>			m_Shader;
	public:
		TestVertexArray();
		~TestVertexArray();

		virtual void OnUpdate(float deltaTime) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;

		virtual void OnEvent(Event& e) override;
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		void OnViewportResize(glm::vec2 viewportSize);
	};

}