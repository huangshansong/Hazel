#pragma once
#include "hzpch.h"

#include "Core.h"

namespace Hazel
{
	class HAZEL_API HObject
	{
	public:
		HObject();

		~HObject();
		
		static unsigned int getHObjectIDCount() { return HObject::s_HObjectIDCount; }

		unsigned int m_HObjectID;
		
	protected:
		
		static unsigned int s_HObjectIDCount;

		static std::unordered_set<unsigned int> s_HObjectIDSet;

	};
}


