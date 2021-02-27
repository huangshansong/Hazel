#pragma once

#include "Hazel/Layer.h"

namespace Hazel {

	//layer ImGui handles the events by itself
	//and it sends the events to other layers
	class HAZEL_API ImGuiLayer :public Layer {
	public:
		
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnRender() override;
		virtual void OnUpdate() override;
		
		void Begin();
		void End();

	protected:
		std::string m_DebugName = "ImGuiLayer";
	private:
		float m_Time = 0.0f;
	};

}