#pragma once
#include "Glados/Core/Window.h"

#include "GLFW/glfw3.h"

namespace Glados {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() override;

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width;
			unsigned int Height;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}


