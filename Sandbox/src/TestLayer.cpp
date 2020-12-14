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
	lib.Load("res/shaders/textureTest.shader");

	FramebufferSpecification spec;
	spec.Width = 1280;
	spec.Height = 720;
	m_Framebuffer = Framebuffer::Create(spec);
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
	m_Framebuffer->Bind();
	Renderer::Clear();
	if (m_CurrentTest)
		m_CurrentTest->OnRender();
	m_Framebuffer->Unbind();
}

void SandboxLayer::OnImGuiRender()
{
	static bool open_dockspace = true;
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &open_dockspace, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows,
			// which we can't undo at the moment without finer window depth/z control.
			ImGui::MenuItem("Exit");
			
			ImGui::Separator();
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

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

	ImGui::Begin("Scene");
	uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
	ImGui::Image((void*)textureID, ImVec2((int)1280 / 2, (int)720 / 2), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();

	ImGui::End();
}

void SandboxLayer::OnEvent(Glados::Event& e)
{
	if (m_CurrentTest && !ImGui::IsAnyWindowHovered())
		m_CurrentTest->OnEvent(e);
}
