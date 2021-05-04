#pragma once
#include "Test.h"
#include "glm/glm.hpp"

using namespace glm;

namespace test {

	class Test3D : public Test 
	{
	private:
		Ref<VertexArray> m_VertexArray;
		Ref<IndexBuffer> m_IndexBuffer;
		Ref<VertexBuffer> m_VertexBuffer;
		mat4 m_Transform;
		mat4 m_View, m_Projection;
		Ref<Shader> m_Shader;

		vec3 m_Position;
		vec3 m_Rotation;
		float m_RotSpd;
		vec3 m_Scale;

		Ref<Mesh> m_Mesh;
	public:
		Test3D();
		~Test3D();

		virtual void OnUpdate(float timestep) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;
		void OnViewportResize(glm::vec2 viewportSize) override;

	private:
		bool OnKeyPressedEvent(KeyPressedEvent& e);
	};

}