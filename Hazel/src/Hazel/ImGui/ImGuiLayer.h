#pragma once

#include "Hazel/Layer.h"

namespace Hazel {

	//layer ImGui handles the events by itself
	//and it sends the events to other layers
	class HAZEL_API ImGuiLayer :public Layer {
		friend class Application;
	private:
		
		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onRender() override;
		virtual void onUpdate() override;
		
		void begin();
		void end();

	protected:
		std::string m_DebugName = "ImGuiLayer";
	private:
		float m_Time = 0.0f;
	};

}