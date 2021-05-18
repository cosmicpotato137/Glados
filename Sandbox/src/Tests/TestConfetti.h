#pragma once
#include "Test.h"
#include "glm/glm.hpp"
// TODO: add Sandbox/src to includedirs
#include "../ParticleSystems/Confetti.h"

using namespace glm;

namespace test {

	class TestConfetti : public Test
	{
	private:
		EditorCamera m_Camera;
		Confetti m_Particles;

	public:
		TestConfetti();
		~TestConfetti();

		virtual void OnUpdate(float timestep) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;
		void OnViewportResize(glm::vec2 viewportSize) override;
	};

}