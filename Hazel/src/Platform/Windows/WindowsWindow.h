#pragma once
#include "hzpch.h"

#include <GLFW/glfw3.h>

#include "Hazel/Window.h"
#include "Hazel/Viewport.h"

namespace Hazel {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(void* application, const WindowProps& props);

		virtual ~WindowsWindow() override;

		virtual bool isWindowClosed() const override { return m_WindowClosed; }

		virtual uint32_t getWidth() const override { return m_Data.Width; }

		virtual uint32_t getHeight() const override { return m_Data.Height; }

		virtual void* getNativeWindow() const override { return m_Window; }
		
		virtual void init(const WindowProps& props);

		virtual void shutdown();
		
		// Window attributes
		virtual void setEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
	
		// Window attributes
		virtual void setVSync(bool enabled) override;

		virtual bool isVSync() const override;

		virtual void onUpdate() override;

		virtual void onRender() override;

		virtual void onPlayerInputEvent(Event&) override;

		virtual bool onWindowClose(WindowCloseEvent&) override;
	protected:
		GLFWwindow* m_Window;
	};

}