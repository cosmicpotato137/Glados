#pragma once
#include "Glados/Core/Window.h"

#include "Glados/Renderer/GraphicsContext.h"
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

		virtual inline void* GetNativeWindow() const override { return m_GLFWWindow; }
		virtual float GetWindowTime() const override { return (float)glfwGetTime(); }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_GLFWWindow;
		GraphicsContext* m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width;
			unsigned int Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}


