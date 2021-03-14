#include "hzpch.h"

#include "badAllocHandler.h"

using namespace std;
namespace Hazel
{
	void BadAllocHandler::noMoreMemory()
	{
		cerr << "unable to satisfy request for memory\n";

		abort();
	}
}