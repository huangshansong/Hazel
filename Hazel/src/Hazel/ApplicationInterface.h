#pragma once

#include "Core.h"
#include "Application.h"

namespace Hazel
{
	class HAZEL_API ApplicationInterface
	{
	public:
		//to be defined in CLIENT
		static Application* create();
		static void run(Application* application);
		static void destroy(Application* application);
	};
}