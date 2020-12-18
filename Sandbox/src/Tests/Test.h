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
		TestMenu(Test*& currentTestPointer);
		~TestMenu();

		void OnRender() override;
		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			GD_CORE_TRACE("Registering Test: {0}", name);

			m_Tests.push_back(std::make_pair(name, []() { return new T(); }));
		}

	private:
		Test*& m_CurrentTest;
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;
	};

}