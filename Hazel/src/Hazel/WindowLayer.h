#pragma once

#include "Layer.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Window.h"

namespace Hazel {

	class HAZEL_API WindowLayer :public Layer {
		friend class Application;
	public:
	private:

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onRender() override;
		virtual void onEvent(Event& event) override;
		virtual void onUpdate() override;

		void begin();
		void end();

	protected:
		std::string m_DebugName = "WindowLayer";

	private:
		bool onWindowClose(WindowCloseEvent& e);

	};

}