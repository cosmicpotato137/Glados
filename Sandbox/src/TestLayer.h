#pragma once
#include "Glados.h"
#include "Tests/Test.h"

using namespace test;

class SandboxLayer : public Glados::Layer
{
private:
	Test* m_CurrentTest;
	TestMenu* m_TestMenu;
public:
	SandboxLayer();
	~SandboxLayer();

	void OnAttach() override;

	void OnUpdate(Glados::Timestep timestep) override;
	void OnRender() override;
	void OnImGuiRender() override;

	void OnEvent(Glados::Event& e);
};
