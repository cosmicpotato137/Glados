#pragma once
#include "Test.h"
#include "glm/glm.hpp"

using namespace glm;

namespace test {

	class Test3D : public Test 
	{
	private:
		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;
		mat4 m_View;
		mat4 m_Projection;

		vec3 m_Position;
		vec3 m_Rotation;
		vec3 m_Scale;
		mat4 m_Model;

		Ref<Material> m_Material;
		Ref<Mesh> m_Mesh;
	public:
		Test3D();
		~Test3D();

		virtual void OnUpdate(float timestep) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

	private:
		bool OnWindowResizeEvent(WindowResizeEvent& e);
	};

}