#include "gladospch.h"
#include "GDApplication.h"
#include "Glados/Renderer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//#include "Renderer.h"


namespace Glados {

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application::Application()
	{
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
		// init gl library
		// TODO: abstract this!
        if (glewInit() != GLEW_OK)
            GD_CORE_ERROR("Error: GLEW_NOT_OK");

        GD_CORE_INFO(glGetString(GL_VERSION));

		glClearColor(1, 0, 1, 1);
		while (m_Running)
		{
			m_Window->OnUpdate();
			glClear(GL_COLOR_BUFFER_BIT);
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(Application::OnKeyPressed));
		GD_CORE_TRACE("{0}", e);
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
}