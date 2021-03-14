#pragma once

#include "Hazel/Events/Event.h"
#include "ImGui/MyImGui.h"

#include "HObject.h"
#include "Viewport.h"
#include "Core.h"

namespace Hazel {

	struct HAZEL_API WindowProps : public HObject
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Hazel Engine",
			        uint32_t width = 1920,
			        uint32_t height = 1080)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	class HAZEL_API Window : public HObject
	{
	public:
		static Window* create(void* application, const WindowProps& props = WindowProps());

		virtual ~Window() = default;

		virtual bool isWindowClosed() const = 0;

		virtual uint32_t getWidth() const = 0;

		virtual uint32_t getHeight() const = 0;

		virtual void* getNativeWindow() const = 0;


		// Window attributes
		using EventCallbackFn = std::function<void(Event&)>;

		virtual void setEventCallback(const EventCallbackFn&) = 0;

		// Window attributes
		virtual void setVSync(bool) = 0;

		virtual bool isVSync() const = 0;

		virtual void onUpdate() = 0;

		virtual void onRender() = 0;

		virtual void onPlayerInputEvent(Event&) = 0;

		virtual bool onWindowClose(WindowCloseEvent&) = 0;

		std::vector<std::shared_ptr<Viewport>> m_Viewports;

		std::unique_ptr<MyImGui> m_ImGui;

	protected:
		void* m_OfApplication;
		bool m_WindowClosed;

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
