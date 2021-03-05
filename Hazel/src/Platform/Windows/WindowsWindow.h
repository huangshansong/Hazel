#pragma once

#include <GLFW/glfw3.h>

#include "Hazel/Window.h"
#include "Hazel/Viewport.h"

namespace Hazel {

	class WindowsWindow : public Window
	{
		friend class WindowLayer;
		friend class Window;
	public:
		unsigned int getWidth() const override { return m_Data.Width; }
		unsigned int getHeight() const override { return m_Data.Height; }
		virtual void* getNativeWindow() const override { return m_Window; }
		virtual Viewport* getViewport() const override { return m_Viewport; }

		// Window attributes
		void setEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		virtual void setViewport(Viewport* viewport) override { m_Viewport = viewport; }

	private:
		WindowsWindow(const WindowProps& props);
		virtual void init(const WindowProps& props);
		virtual void shutdown();
		virtual ~WindowsWindow();

		virtual void onRender() override;

		// Window attributes
		void setVSync(bool enabled) override;
		bool isVSync() const override;

		
		GLFWwindow* m_Window;
		Viewport* m_Viewport;
		
		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}