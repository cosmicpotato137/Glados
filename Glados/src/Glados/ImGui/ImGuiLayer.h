#pragma once
#include "Glados/Core/Layer.h"
#include "Glados/Events/ApplicationEvent.h"
#include "imgui.h"
#include "Glados/Events/MouseEvent.h"
#include "Glados/Events/KeyEvent.h"

namespace Glados {	

	class GLADOS_API ImGuiLayer : public Layer
	{
	private:
		float m_Time;
	protected:
		ImGuiContext* m_ImGuiContext;
	public:
		ImGuiLayer(const std::string& name = "ImGui Layer");
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(Timestep ts) override;
		void OnImGuiRender() override;
		void OnEvent(Event& e) override;

		void Begin();
		void End();

	protected:
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

