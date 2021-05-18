#pragma once
#include "Test.h"
#include "glm/glm.hpp"
// TODO: add Sandbox/src to includedirs
#include "../ParticleSystems/Confetti.h"
#include "../ParticleSystems/BoidTest.h"

using namespace glm;

namespace test {

	class TestBoid : public Test
	{
	private:
		EditorCamera m_Camera;
		BoidTest m_Particles;

	public:
		TestBoid();
		~TestBoid();

		virtual void OnUpdate(float timestep) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;
		void OnViewportResize(glm::vec2 viewportSize) override;
	};

}