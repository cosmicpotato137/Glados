#pragma once

#include "Core.h"
#include "Glados/Events/Event.h"
#include "Timestep.h"

namespace Glados {

	class GLADOS_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep timestep) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}

		inline const std::string GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};

}	