#include "TestLayer.h"
#include "Tests/TestClearColor.h"
#include "Tests/TestVertexArray.h"
#include "Tests/TestTexture.h"
#include "Tests/Test3D.h"
#include "Tests/TestParticleSystem.h"
//#include "Tests/TestPhong.h"

TestLayer::TestLayer()
	: Layer("Testing Overlay")
{
}

TestLayer::~TestLayer()
{
}

void TestLayer::OnAttach()
{
	m_TestMenu = new TestMenu();
	m_CurrentTest = m_TestMenu;

	RegisterTest<TestClearColor>("Test Clear Color");
	RegisterTest<TestVertexArray>("Test Vertex Array");
	RegisterTest<TestTexture2D>("2D Texture Test");
	RegisterTest<Test3D>("3D Test");
	RegisterTest<TestParticleSystem>("Particle System 1");

	// initiallize shaders
	ShaderLibrary& lib = Renderer::GetShaderLibrary();
	lib.Load("res/shaders/basic.shader");
	lib.Load("res/shaders/basic2d.shader");
	lib.Load("res/shaders/debug.shader");
	lib.Load("res/shaders/forcefield.shader");
	lib.Load("res/shaders/Gourad.shader");
	lib.Load("res/shaders/Phong.shader");
	lib.Load("res/shaders/textureTest.shader");

	int filter = EventCategory::EventCategoryApplication | EventCategory::EventCategoryInput;
	bool outcome = EventCategory::EventCategoryApplication & filter;

	FramebufferSpecification fbSpec;
	fbSpec.Width = 1280;
	fbSpec.Height = 720;
	m_Framebuffer = Framebuffer::Create(fbSpec);
}

void TestLayer::OnDetach()
{
	if (m_CurrentTest != m_TestMenu)
		delete m_CurrentTest;
	delete m_TestMenu;
}

void TestLayer::OnUpdate(Timestep timestep)
{
	// Resize
	if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
		m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
		(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
	{
		m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_CurrentTest->OnViewportResize(glm::vec2(m_ViewportSize.x, m_ViewportSize.y));
	}

	if (m_CurrentTest)
		m_CurrentTest->OnUpdate(0.0f);

	// event blocking
	int filter = EventCategory::None;
	if (!m_ViewportFocused && !m_ViewportHovered)
		filter = EventCategory::EventCategoryKeyboard | EventCategory::EventCategoryMouse;
	else if (!m_ViewportHovered)
		filter = EventCategory::EventCategoryMouse;
	else if (!m_ViewportFocused)
		filter = EventCategory::EventCategoryKeyboard;

	Application::Get().GetImGuiLayer().SetEventFilter((EventCategory)filter);
}

void TestLayer::OnRender()
{
	m_Framebuffer->Bind();
	RenderCommand::Clear();
	if (m_CurrentTest)
		m_CurrentTest->OnRender();
	m_Framebuffer->Unbind();
}

void TestLayer::OnImGuiRender()
{
	Dockspace();

	// viewport window
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("Viewport");
	m_ViewportHovered = ImGui::IsWindowHovered();
	m_ViewportFocused = ImGui::IsWindowFocused();

	// change viewport size
	ImVec2 viewportPos = ImGui::GetWindowPos();
	ImVec2 viewportSize = ImGui::GetContentRegionAvail();
	if (m_ViewportSize != *(glm::vec2*) & viewportSize)
	{
		Renderer::OnWindowResize((int)viewportSize.x, (int)viewportSize.y);
		m_ViewportSize = *(glm::vec2*) &viewportSize;
		if (m_CurrentTest)
			m_CurrentTest->OnViewportResize(m_ViewportSize);
	}

	// write framebuffer to imgui window
	uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
	ImGui::Image((void*)textureID, viewportSize, ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
	ImGui::PopStyleVar();

	// demo window
	static bool showDemo = false;
	if (showDemo)
		ImGui::ShowDemoWindow(&showDemo);

	// test windows
	ImGui::Begin("Test");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	if (m_CurrentTest != m_TestMenu && ImGui::Button("<-"))
	{
		delete m_CurrentTest;
		m_CurrentTest = m_TestMenu;
	}
	else if (m_CurrentTest == m_TestMenu)
	{
		for (auto& test : m_Tests)
		{
			if (ImGui::Button(test.first.c_str()))
			{
				m_CurrentTest = test.second();
				m_CurrentTest->OnViewportResize(m_ViewportSize);
			}
		}
	}
	m_CurrentTest->OnImGuiRender();
	ImGui::End();
}

void TestLayer::OnEvent(Glados::Event& e)
{
	if (m_CurrentTest)
		m_CurrentTest->OnEvent(e);
}

void TestLayer::Dockspace()
{
	static bool p_open = true;
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->GetWorkPos());
	ImGui::SetNextWindowSize(viewport->GetWorkSize());
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::Begin("DockSpace Demo", &p_open, window_flags);
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
			if (ImGui::MenuItem("Exit"))
				Application::Get().Close();

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::End();
}

template<typename T>
void TestLayer::RegisterTest(const std::string& name)
{
	GD_CORE_TRACE("Registering Test: {0}", name);

	m_Tests.push_back(std::make_pair(name, []() { return new T(); }));
}