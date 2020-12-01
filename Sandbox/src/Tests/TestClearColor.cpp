#include "TestClearColor.h"

namespace test {

	TestClearColor::TestClearColor()
		: m_ClearColor(0.5f, 0.5f, 0.5f, 1.0f)
	{
	}

	TestClearColor::~TestClearColor()
	{
	}

	void TestClearColor::OnUpdate(float deltaTime)
	{
		Renderer::SetClearColor(m_ClearColor);
	}

	void TestClearColor::OnRender()
	{
	}

	void TestClearColor::OnImGuiRender()
	{
		ImGui::ColorEdit4("Clear Color", &m_ClearColor[0]);
	}
}