#pragma once

#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"

#include "Layer.h"

namespace Hazel {

	class HAZEL_API ViewportLayer :public Layer {
		friend class Application;
	private:

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onRender() override;
		virtual void onEvent(Event& event) override;
		virtual void onUpdate() override;

		void begin();
		void end();

	
	protected:
		std::string m_DebugName = "ViewportLayer";

	private:
		bool onFramebufferResize(FramebufferResizeEvent& e);
		bool onKeyPressed(KeyPressedEvent& e);

	};

}