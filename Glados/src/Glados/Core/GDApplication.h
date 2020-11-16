#pragma once
#include "gladospch.h"

#include "Core.h"
#include "Window.h"
#include "Glados/Events/ApplicationEvent.h"
#include "Glados/Events/MouseEvent.h"
#include "Glados/Events/KeyEvent.h"

class GLFWwindow;

namespace Glados {

	class GLADOS_API Application
	{
	private:
		Window* m_Window;
		bool m_Running = true;
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);
	};

	// to be defined in client
	Application* CreateApplication();

}