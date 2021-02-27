#include "hzpch.h"

#include "Actor.h"
#include "Hazel/Events/Event.h"

namespace Hazel
{
	void Actor::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
	}

	void Actor::OnUpdate()
	{
	}
}

