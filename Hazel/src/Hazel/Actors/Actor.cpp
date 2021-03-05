#include "hzpch.h"

#include "Hazel/Events/Event.h"
#include "Hazel/Application.h"
#include "Hazel/Log.h"
#include "Hazel/Core.h"

#include "Actor.h"

namespace Hazel
{
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
		glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
			//modelTrans = glm::translate(modelTrans, model->transform); 
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));//should be replaced by actor's transform later!!!
		for (Mesh* mesh : m_Model->m_Meshes)
		{
			mesh->getShader()->use();
			//HZ_CORE_INFO("shader->use();");
			mesh->getShader()->setMat4("model", model);
			//HZ_CORE_INFO("shader->setMat4(model);");

		}
		
		m_Model->draw();
	}
}

