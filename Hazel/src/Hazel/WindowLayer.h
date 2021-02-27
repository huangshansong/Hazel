#pragma once

#include "Layer.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"


namespace Hazel {

	class HAZEL_API WindowLayer :public Layer {
	public:
		

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnRender() override;
		virtual void OnEvent(Event& event) override;
		virtual void OnUpdate() override;

		void Begin();
		void End();

	protected:
		std::string m_DebugName = "WindowLayer";

	private:
		bool OnWindowClose(WindowCloseEvent& e);

	};

}