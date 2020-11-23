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
	protected:
	public:
		ImGuiLayer(const std::string& name = "ImGui Layer");
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(Timestep ts) override;

#define IMGUI_SHOW_DEMO
		void OnImGuiRender() override;

		void Begin();
		void End();
	};

}

