#pragma once
#include "Test.h"
#include "glm/glm.hpp"

using namespace glm;

namespace test {

	class TestParticleSystem : public Test
	{
	private:
		EditorCamera m_Camera;

	public:
		TestParticleSystem();
		~TestParticleSystem();

		virtual void OnUpdate(float timestep) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;
		void OnViewportResize(glm::vec2 viewportSize) override;
	};

}