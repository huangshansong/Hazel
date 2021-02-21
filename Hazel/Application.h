#pragma once

#include "core.h"

namespace Hazel {
	class HAZEL_API Application
	{
	public:
		virtual ~Application();
		void Run();
	};

	//to be defined in CLIENT
	Application* CreateApplication();
}

