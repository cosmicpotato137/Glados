#pragma once
#include "Test.h"
#include "glm/glm.hpp"
// TODO: add Sandbox/src to includedirs
#include "../ParticleSystems/BoidTest2.h"

using namespace glm;

namespace test {

	class TestBoid2 : public Test
	{
	private:
		EditorCamera m_Camera;
		BoidTest2 m_Particles;

	public:
		TestBoid2();
		~TestBoid2();

		virtual void OnUpdate(float timestep) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;
		void OnViewportResize(glm::vec2 viewportSize) override;
	};

}