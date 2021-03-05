#pragma once

#include "Core.h"
#include "Layer.h"

namespace Hazel {

	class HAZEL_API LayerStack
	{
		friend class Application;
	private:
		LayerStack();
		~LayerStack();

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);
		void popLayer(Layer* layer);
		void popOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
		
	private:
		std::vector<Layer*> m_Layers;
		unsigned int  m_LayerInsertIndex;
	};

}