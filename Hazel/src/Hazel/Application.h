#pragma once
#include "hzpch.h"

#include "Core.h"
#include "Window.h"
#include "WindowLayer.h"
#include "LayerStack.h"


namespace Hazel {
	class HAZEL_API Application
	{
	public:
		Application();
		virtual ~Application() {};
		void run();

		inline static Window* getWindow() { return s_Window; }
		inline static float getDeltaTime() { return s_DeltaTime; }
		inline static bool getCanDisplayTest() { return s_CanDisplayTest; }

		class WindowSetter
		{
			friend class WindowLayer;
		private:
			inline static void setWindow(Window* window) { Application::s_Window = window; }
		};
		class WindowCloseSetter
		{
			friend class WindowLayer;
		private:
			inline static void setWindowClose(bool windowClose) { Application::s_WindowClose = windowClose; }
		};

	protected:
		inline static void setWindow(Window* window) { s_Window = window; }
		void onEvent(Event& e);
		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);

	private:
		static Window* s_Window;
		static float s_DeltaTime;
		static bool s_WindowClose;
		static bool s_CanDisplayTest;
		bool m_Running;
		float m_LastFrameTime;
		LayerStack m_LayerStack;
	};

	//to be defined in CLIENT
	Application* createApplication();
}

