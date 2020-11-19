#include "gladospch.h"
#include "GDApplication.h"
#include "Glados/Renderer.h"
#include "Input.h"

//TODO: make Glados key list
#include "GLFW/glfw3.h"

namespace Glados {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		GD_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
        Renderer::Init();
		m_Window = Window::Create();
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application()
	{
		delete m_Window;
	}

	void Application::Run()
	{
        GD_CORE_INFO(glGetString(GL_VERSION));

		glClearColor(1, 0, 1, 1);
		while (m_Running)
		{

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			auto[x, y] = Input::GetMousePos();
			GD_CORE_INFO("Mouse Pos: ({0}, {1})", x, y);

			m_Window->OnUpdate();
			glClear(GL_COLOR_BUFFER_BIT);
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(Application::OnKeyPressed));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
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
		case GLFW_KEY_ESCAPE:
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