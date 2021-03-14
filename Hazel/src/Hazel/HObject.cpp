#include "hzpch.h"

#include "HObject.h"

using namespace std;
namespace Hazel
{
	unsigned int HObject::s_HObjectIDCount = 0;
	unordered_set<unsigned int> HObject::s_HObjectIDSet;

	HObject::HObject()
	{
		m_HObjectID = s_HObjectIDCount++; 
		s_HObjectIDSet.insert(m_HObjectID);
	}
	HObject::~HObject()
	{
		s_HObjectIDSet.erase(m_HObjectID);
	}
}
