#pragma once
#include "Glados.h"
#include "imgui/imgui.h"

using namespace Glados;

namespace test {

	class Test
	{
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}
		virtual void OnViewportResize(glm::vec2 viewportSize) {}
	};

	class TestMenu : public Test
	{
	public:
		TestMenu();
		~TestMenu();

		void OnRender() override;
		void OnImGuiRender() override;
	};

}