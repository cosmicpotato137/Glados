#include "gladospch.h"
#include "GDApplication.h"
#include "Glados/Renderer.h"
#include "Input.h"
#include "Timestep.h"

namespace Glados {

	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_LastFrameTime(0.0f)
	{
		GD_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		m_Window = Window::Create();
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		GD_CORE_INFO("Welcome to Glados!");
        Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		delete m_Window;
	}

	void Application::Run()
	{
        GD_CORE_INFO(glGetString(GL_VERSION));

		while (m_Running)
		{
			float time = GetTime();
			Timestep timestep = time - m_LastFrameTime;

			// update per layer
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timestep);

			// imgui rendering per layer
			m_ImGuiLayer->Begin();

			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
			glClear(GL_COLOR_BUFFER_BIT);
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}

		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(Application::OnKeyPressed));
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnKeyPressed(KeyPressedEvent& e)
	{
		switch (e.GetKeyCode())
		{
		case GD_KEY_ESCAPE:
			m_Running = false;
		default:
			break;
		}

		return true;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}
}