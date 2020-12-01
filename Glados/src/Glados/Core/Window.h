#pragma once
#include <string>
#include "Glados/Events/Event.h"

namespace Glados {
	
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Glados Renderer", unsigned int width = 1280, unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() = 0;
		
		virtual void* GetNativeWindow() const = 0;
		virtual float GetWindowTime() const = 0;

		static void SetClipboardText(void* user_data, const char* text);
		static const char* GetClipboardText(void* user_data);

		static Window* Create(const WindowProps& props = WindowProps());
	};

}