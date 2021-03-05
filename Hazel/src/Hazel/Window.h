#pragma once

#include "Hazel/Events/Event.h"

#include "Viewport.h"
#include "Core.h"

namespace Hazel {

	struct HAZEL_API WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Hazel Engine",
			        uint32_t width = 1600,
			        uint32_t height = 900)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	// Interface representing a desktop system based Window
	class HAZEL_API Window
	{
		friend class WindowLayer;
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		
		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;
		virtual void* getNativeWindow() const = 0;
		virtual Viewport* getViewport() const = 0;
		virtual void setViewport(Viewport*) = 0;

		// Window attributes
		virtual void setEventCallback(const EventCallbackFn& callback) = 0;

		
	protected:
		static Window* create(const WindowProps& props = WindowProps());
		virtual ~Window() = default;

		virtual void onRender() = 0;

		// Window attributes
		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSync() const = 0;

	};

}
