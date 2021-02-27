#pragma once

#include "Hazel/Layer.h"

namespace Hazel {

	class HAZEL_API LevelLayer :public Layer {
	public:

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnRender() override;
		virtual void OnEvent(Event& event) override;
		virtual void OnUpdate() override;


	protected:
		std::string m_DebugName = "LevelLayer";
	};

}