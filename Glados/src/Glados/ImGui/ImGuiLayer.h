#pragma once
#include "Glados/Core/Layer.h"
#include "Glados/Events/ApplicationEvent.h"
#include "Glados/Events/MouseEvent.h"
#include "Glados/Events/KeyEvent.h"

namespace Glados {	

	class ImGuiLayer : public Layer
	{
	private:
		float m_Time;
		EventCategory m_EventFilters;
	public:
		ImGuiLayer(const std::string& name = "ImGui Layer");
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(Timestep ts) override;
		void OnEvent(Event& e) override;

		void OnImGuiRender() override;

		void Begin();
		void End();

		void SetEventFilter(EventCategory category) { m_EventFilters = category; }
	};

}

