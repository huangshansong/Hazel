#pragma once

#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"

#include "Layer.h"

namespace Hazel {

	class HAZEL_API ViewportLayer :public Layer {
	public:

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnRender() override;
		virtual void OnEvent(Event& event) override;
		virtual void OnUpdate() override;

		void Begin();
		void End();


		bool OnFramebufferResize(FramebufferResizeEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);

	protected:
		std::string m_DebugName = "ViewportLayer";
	};

}