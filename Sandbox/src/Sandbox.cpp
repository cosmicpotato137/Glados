#include "Glados.h"
#include "Glados/Core/EntryPoint.h"
#include "imgui/imgui.h"
#include "Tests/TestClearColor.h"

using namespace Glados;
using namespace test;

class SandboxLayer : public Glados::Layer
{
private:
	Test* m_CurrentTest;
	TestMenu* m_TestMenu;
public:
	SandboxLayer()
		: Layer("Testing Overlay")
	{
		m_TestMenu = new TestMenu(m_CurrentTest);
		m_CurrentTest = m_TestMenu;

		m_TestMenu->RegisterTest<TestClearColor>("Clear Color");
	}

	void OnAttach() override
	{
	}

	void OnUpdate(Timestep timestep) override
	{

	}

	void OnImGuiRender() override
	{
		if (m_CurrentTest)
		{
			m_CurrentTest->OnUpdate(0.0f);
			m_CurrentTest->OnRender();

			ImGui::Begin("Test");
			if (m_CurrentTest != m_TestMenu && ImGui::Button("<-"))
			{
				delete m_CurrentTest;
				m_CurrentTest = m_TestMenu;
			}
			m_CurrentTest->OnImGuiRender();
			ImGui::End();
		}

		//ImGui::Begin("Test");
		//ImGui::Button("hi", ImVec2(30, 30));
		//ImGui::End();
	}

};

class Sandbox : public Glados::Application
{
public:
	Sandbox()
		: Application()
	{
		PushLayer(new SandboxLayer());

		//PushOverlay(new ImGuiLayer());
	}

	~Sandbox()
	{
	}
};

Glados::Application* Glados::CreateApplication()
{
	return new Sandbox();
}