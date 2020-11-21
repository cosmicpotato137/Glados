#include "gladospch.h"
#include "ImGuiLayer.h"
#include "Platform/OpenGL/imgui_impl_opengl3.h"
#include "Platform/GLFW/imgui_impl_glfw.h"
#include "glad/glad.h"
#include "Glados/Core/GDApplication.h"
#include "Glados/Core/Input.h"

namespace Glados {

	static bool showDemo = false;

	ImGuiLayer::ImGuiLayer(const std::string& name)
		: Layer(name), m_Time(0.0f), m_ImGuiContext(nullptr)
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
		OnDetach();
	}

	void ImGuiLayer::OnAttach()
	{
		m_ImGuiContext = ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		io.SetClipboardTextFn = Window::SetClipboardText;
		io.GetClipboardTextFn = Window::GetClipboardText;

		io.KeyMap[ImGuiKey_Tab]			= GD_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow]	= GD_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow]	= GD_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow]		= GD_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow]	= GD_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp]		= GD_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown]	= GD_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home]		= GD_KEY_HOME;
		io.KeyMap[ImGuiKey_End]			= GD_KEY_END;
		io.KeyMap[ImGuiKey_Insert]		= GD_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete]		= GD_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace]	= GD_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space]		= GD_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter]		= GD_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape]		= GD_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_KeyPadEnter] = GD_KEY_KP_ENTER;
		io.KeyMap[ImGuiKey_A]			= GD_KEY_A;
		io.KeyMap[ImGuiKey_C]			= GD_KEY_C;
		io.KeyMap[ImGuiKey_V]			= GD_KEY_V;
		io.KeyMap[ImGuiKey_X]			= GD_KEY_X;
		io.KeyMap[ImGuiKey_Y]			= GD_KEY_Y;
		io.KeyMap[ImGuiKey_Z]			= GD_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410");
		GLFWwindow* w = (GLFWwindow*)app.GetWindow().GetNativeWindow();
		ImGui_ImplGlfw_InitForOpenGL(w, false);
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui::DestroyContext(m_ImGuiContext);
	}

	void ImGuiLayer::OnUpdate(Timestep ts)
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();

		float time = app.GetTime();
		io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
		m_Time = time;
	}

	void ImGuiLayer::OnImGuiRender()
	{
#ifdef IMGUI_SHOW_DEMO && GD_DEBUG
		if (showDemo)
			ImGui::ShowDemoWindow(showDemo);
#endif
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseScrollEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseScrollEvent));
		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch<KeyTypedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;
		return true;
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;
		return true;
	}

	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());
		return true;
	}

	bool ImGuiLayer::OnMouseScrollEvent(MouseScrollEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheel = e.GetYOffset();
		io.MouseWheelH = e.GetXOffset();
		return true;
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = true;

		io.KeyCtrl	= io.KeysDown[GD_KEY_LEFT_CONTROL]	|| io.KeysDown[GD_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GD_KEY_LEFT_SHIFT]	|| io.KeysDown[GD_KEY_RIGHT_SHIFT];
		io.KeyAlt	= io.KeysDown[GD_KEY_LEFT_ALT]		|| io.KeysDown[GD_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GD_KEY_LEFT_SUPER]	|| io.KeysDown[GD_KEY_RIGHT_SUPER];

#ifdef IMGUI_SHOW_DEMO && GD_DEBUG
		GD_CORE_INFO(e);
		if (e.GetKeyCode() == GD_KEY_H)
		{
			showDemo = !showDemo;
			GD_CORE_INFO(showDemo);
		}
#endif

		return true;
	}

	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = false;
		return true;
	}

	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		int keycode = e.GetKeyCode();

		if (keycode > 0 && keycode < 0x10000)
			io.AddInputCharacter((unsigned int)keycode);

		return true;
	}

	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1, 1);
		GLCall(glViewport(0, 0, e.GetWidth(), e.GetHeight()));

		return false;
	}
}