#pragma once
#include <vector>
#include "Layer.h"

namespace Glados {

	class LayerStack
	{
	private:
		std::vector<Layer*> m_Layers;
		unsigned int m_LayerInsertIndex;
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	};

}