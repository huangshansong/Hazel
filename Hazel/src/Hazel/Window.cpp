#include "hzpch.h"

#include "Window.h"

#ifdef HZ_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Hazel
{
	Window* Window::create(void* application, const WindowProps& props)
	{
	#ifdef HZ_PLATFORM_WINDOWS
		return new WindowsWindow(application, props);
	#else
		HZ_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
	#endif
	}
}