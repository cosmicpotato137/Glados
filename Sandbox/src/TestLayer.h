#pragma once
#include "Glados.h"
#include "Tests/Test.h"

using namespace test;

class SandboxLayer : public Layer
{
private:
	Test* m_CurrentTest;
	TestMenu* m_TestMenu;
	Ref<Framebuffer> m_Framebuffer;
public:
	SandboxLayer();
	~SandboxLayer();

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(Timestep timestep) override;
	void OnRender() override;
	void OnImGuiRender() override;

	void OnEvent(Event& e);
};