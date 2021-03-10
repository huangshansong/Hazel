#pragma once

#include "Layer.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Window.h"

namespace Hazel {

	class HAZEL_API WindowLayer :public Layer {
		friend class Application;
		friend class ViewportLayer;
	private:

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onRender() override;
		virtual void onEvent(Event& event) override;
		virtual void onUpdate() override;

		void begin();
		void end();

		static void setEventCallback(Window* window, const std::function<void(Event&)>& EventCallbackFn)
			{window->setEventCallback(EventCallbackFn);}
		static void setViewport(Window* window, Viewport* viewport) { window->setViewport(viewport); }

	protected:
		std::string m_DebugName = "WindowLayer";

	private:
		bool onWindowClose(WindowCloseEvent& e);

	};

}