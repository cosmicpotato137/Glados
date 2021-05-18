#pragma once
#include "Test.h"
#include "glm/glm.hpp"
#include "Glados/DataStructures/KdTree.h"
#include "Glados/DataStructures/Agent.h"

namespace test {
	
	class TestClearColor : public Test
	{
	public:
		TestClearColor();
		~TestClearColor();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		glm::vec4 m_ClearColor;
		KdTree tree;
		std::vector<Agent*> agents;
	};
}