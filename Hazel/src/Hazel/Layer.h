#pragma once

#include "Hazel/Events/Event.h"

#include "Core.h"

namespace Hazel {

	class HAZEL_API Layer
	{
	public:
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnRender() {}
		virtual void OnEvent(Event& event) {}
		virtual void OnUpdate(){}

		const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName = "Layer";
	};

}