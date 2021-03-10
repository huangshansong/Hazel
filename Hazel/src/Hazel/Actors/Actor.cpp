#include "hzpch.h"

#include "Hazel/ResourceLoading/landscape.h"
#include "Hazel/ResourceLoading/model.h"
#include "Hazel/ResourceLoading/SimpleGeometryModel.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Application.h"
#include "Hazel/Log.h"
#include "Hazel/Core.h"


#include "Actor.h"

using namespace std;
namespace Hazel
{
	Actor::Actor(string name, glm::vec3 transform)
		: m_Name(name), m_Transform(transform)
	{
		if (name == "Backpack")
		{
			m_Model = createModel("resources/objects/backpack/backpack.obj", false);
			HZ_CORE_INFO("Backpack loaded.");
		}
		else if (name == "Landscape")
		{
			m_Model = createLandscapeModel();
			HZ_CORE_INFO("Landscape loaded.");
		}
		else if (name == "Sphere")
		{
			m_Model = createSimpleGeometryModel(SimpleGeometryType::Sphere);
		}
	}
	void Actor::onEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
	}

	void Actor::onUpdate()
	{

		onRender();
	}
	void Actor::onRender()
	{
		if (m_Model != nullptr)
		{
			drawModel(m_Model);
		}
		
	}
}

