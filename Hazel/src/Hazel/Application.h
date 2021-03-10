#pragma once
#include "hzpch.h"

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"


namespace Hazel {
	class HAZEL_API Application
	{
		friend class ApplicationInterface;
	public:
		class HAZEL_API Interface
		{
			friend class WindowLayer;
		private:
			static void setWindow(Window* window) { Application::s_Window = window; }
		};

	public:
		static Window* getWindow() { return s_Window; }
		static float getDeltaTime() { return s_DeltaTime; }
		static bool getCanDisplayTest() { return s_CanDisplayTest; }

		virtual void onEvent(Event& e);

	protected:
		Application();
		virtual ~Application() {};
		virtual void run();

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);

		static Window* s_Window;
		static bool s_CanDisplayTest;
		bool m_Running;
		static float s_DeltaTime;
		static float s_LastFrameTime;
		LayerStack m_LayerStack;
	};
}

