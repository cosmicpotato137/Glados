#pragma once
#include "Core.h"
#include "Window.h"

#include "Glados/Events/ApplicationEvent.h"
#include "Glados/Events/MouseEvent.h"
#include "Glados/Events/KeyEvent.h"

#include "LayerStack.h"
#include "Glados/ImGui/ImGuiLayer.h"

namespace Glados {

	class Application
	{
	private:
		Window* m_Window;
		bool m_Running = true;
		bool m_Minimized = false;
		float m_LastFrameTime;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
		static Application* s_Instance;

	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		static float GetTime() { return s_Instance->GetWindow().GetWindowTime(); }

		static inline Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);
	};

	// to be defined in client
	Application* CreateApplication();

}