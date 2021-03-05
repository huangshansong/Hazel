#pragma once

#include "Hazel/Events/Event.h"

#include "Core.h"

namespace Hazel {

	class HAZEL_API Layer
	{
		friend class Application;
		friend class LayerStack;
	public:
		virtual ~Layer() = default;

	protected:
		
		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onRender() {}
		virtual void onEvent(Event& event) {}
		virtual void onUpdate(){}

		const std::string& getName() const { return m_DebugName; }
	protected:
		std::string m_DebugName = "Layer";
	};

}