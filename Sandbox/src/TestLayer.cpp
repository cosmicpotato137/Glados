#include "TestLayer.h"
#include "Tests/TestClearColor.h"
#include "Tests/TestVertexArray.h"
#include "Tests/TestTexture.h"
#include "Tests/Test3D.h"
//#include "Tests/TestPhong.h"

SandboxLayer::SandboxLayer()
	: Layer("Testing Overlay")
{
}

SandboxLayer::~SandboxLayer()
{
}

void SandboxLayer::OnAttach()
{
	m_TestMenu = new TestMenu(m_CurrentTest);
	m_CurrentTest = m_TestMenu;

	m_TestMenu->RegisterTest<TestClearColor>("Test Clear Color");
	m_TestMenu->RegisterTest<TestVertexArray>("Test Vertex Array");
	m_TestMenu->RegisterTest<TestTexture2D>("2D Texture Test");
	m_TestMenu->RegisterTest<Test3D>("3D Test");

	// initiallize shaders
	ShaderLibrary& lib = Renderer::GetShaderLibrary();
	lib.Load("res/shaders/basic.shader");
	lib.Load("res/shaders/debug.shader");
	lib.Load("res/shaders/forcefield.shader");
	lib.Load("res/shaders/Gourad.shader");
	lib.Load("res/shaders/Phong.shader");
}

void SandboxLayer::OnDetach()
{
	if (m_CurrentTest != m_TestMenu)
		delete m_CurrentTest;
	delete m_TestMenu;
}

void SandboxLayer::OnUpdate(Timestep timestep)
{
	if (m_CurrentTest)
		m_CurrentTest->OnUpdate(0.0f);
}

void SandboxLayer::OnRender()
{
	if (m_CurrentTest)
		m_CurrentTest->OnRender();
}

void SandboxLayer::OnImGuiRender()
{
	if (m_CurrentTest)
	{
		ImGui::Begin("Test");
		if (m_CurrentTest != m_TestMenu && ImGui::Button("<-"))
		{
			delete m_CurrentTest;
			m_CurrentTest = m_TestMenu;
		}
		m_CurrentTest->OnImGuiRender();
		ImGui::End();
	}
}

void SandboxLayer::OnEvent(Glados::Event& e)
{
	if (m_CurrentTest && !ImGui::IsAnyWindowHovered())
		m_CurrentTest->OnEvent(e);
}
