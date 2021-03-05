#pragma once

#include "Hazel/Layer.h"
#include "Hazel/Levels/Level.h"

namespace Hazel {

	class HAZEL_API LevelLayer :public Layer {
		friend class Application;
	public:

	private:

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onRender() override;
		virtual void onEvent(Event& event) override;
		virtual void onUpdate() override;


	protected:
		std::string m_DebugName = "LevelLayer";
	};

}