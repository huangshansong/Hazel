#pragma once
#include "hzpch.h"

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"


namespace Hazel {
	class HAZEL_API Application
	{
	public:
		Application();
		virtual ~Application() {};
		void Run();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		static Window* m_Window;

		
		static float deltaTime;

		static bool m_WindowClose;

	private:
		bool m_Running;
		float lastFrameTime;
		LayerStack m_LayerStack;
	};

	//to be defined in CLIENT
	Application* CreateApplication();
}

