#pragma once
#include "hzpch.h"

#include "Editor/Editor.h"

#include "Window.h"
#include "HObject.h"
#include "Core.h"


namespace Hazel {
	class HAZEL_API Application : public HObject
	{
	public:
		Application();

		virtual ~Application() = default;

		virtual void run();

		static float getDeltaTime() { return s_DeltaTime; }

		virtual void onPlayerInputEvent(Event& e);

		std::vector<std::shared_ptr<Window>> m_Windows;

		std::unique_ptr<Editor> m_Editor;

	protected:
		bool m_Running;
		static float s_DeltaTime;
		static float s_LastFrameTime;
		
	};
	
	Application* createApplication();
}

