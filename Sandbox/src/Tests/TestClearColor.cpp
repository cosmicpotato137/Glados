#include "TestClearColor.h"

namespace test {

	TestClearColor::TestClearColor()
		: m_ClearColor(0.5f, 0.5f, 0.5f, 1.0f)
	{
		agents = {
			new Agent(4, 4.0f),
			new Agent(4, 4.0f),
			new Agent(4, 4.0f),
			new Agent(4, 4.0f),
			new Agent(4, 4.0f)
		};
		tree = KdTree(agents);

		for (Agent* a : agents)
		{
			//a->setPosition(gdm::Random::RandomUnitCube() * 4.0f);
		}

		tree.BuildAgentTree();
		for (Agent* a : agents)
		{
			tree.ComputeAgentNeighbors(a, 4.0f);
		}
	}

	TestClearColor::~TestClearColor()
	{
	}

	void TestClearColor::OnUpdate(float deltaTime)
	{
		RenderCommand::SetClearColor(m_ClearColor);
	}

	void TestClearColor::OnRender()
	{
	}

	void TestClearColor::OnImGuiRender()
	{
		ImGui::ColorEdit4("Clear Color", &m_ClearColor[0]);
	}
}