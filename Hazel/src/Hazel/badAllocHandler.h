#pragma once

#include "Core.h"

namespace Hazel
{
	class HAZEL_API BadAllocHandler
	{
	public:
		static void noMoreMemory();
	};
	
}