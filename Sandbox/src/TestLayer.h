#pragma once
#include "Glados.h"
#include "Tests/Test.h"

using namespace test;
using TestStack = std::vector<std::pair<std::string, std::function<Test* ()>>>;
class TestLayer : public Layer
{
private:
	Test* m_CurrentTest;
	TestMenu* m_TestMenu;
	TestStack m_Tests;
	glm::vec2 m_ViewportSize = { 0, 0 };
	bool m_ViewportHovered = false;
	bool m_ViewportFocused = false;
public:
	TestLayer();
	~TestLayer();

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(Timestep timestep) override;
	void OnRender() override;
	void OnImGuiRender() override;

	void OnEvent(Event& e);

private:
	void Dockspace();
	template<typename T>
	void RegisterTest(const std::string& name);
};
