#include "hzpch.h"


#include "Hazel/Events/Event.h"
#include "Hazel/Application.h"
#include "Hazel/Log.h"
#include "Hazel/Core.h"


#include "Actor.h"

using namespace std;
namespace Hazel
{
	Actor::Actor(void* level, string name, glm::vec3 transform)
		: m_OfLevel(level), m_Name(name), m_Transform(transform)
	{
		((Level*)level)->m_Actors.emplace_back(shared_ptr<Actor>(this));
	}
	void Actor::onPlayerInputEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
	}

	void Actor::onUpdate()
	{
		m_Model->onUpdate();

		onRender();
	}
	void Actor::onRender()
	{
		if (m_Model != nullptr)
		{
			m_Model->draw();
		}
	}
}

