#pragma once
#include "Glados/Core/Layer.h"
#include "Glados/Events/ApplicationEvent.h"
#include "Glados/Events/MouseEvent.h"
#include "Glados/Events/KeyEvent.h"

namespace Glados {

	class GLADOS_API ImGuiLayer : public Layer
	{
	private:
		float m_Time;
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& e);

	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrollEvent(MouseScrollEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);
	};

}

