#pragma once
#include "Hazel/Core.h"
#include "Hazel/HObject.h"

namespace Hazel {

	//layer ImGui handles the events by itself
	//and it sends the events to other layers
	class HAZEL_API MyImGui : public HObject
	{
	public:
		MyImGui(void*);
		
		~MyImGui();

		void onRender();

		void onUpdate();
		
	protected:
		void begin();

		void end();

		void* m_OfWindow;
		float m_Time = 0.0f;
	};

}