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
	void Actor::OnRender()
	{
		for (Model* model : models)
		{
			glm::mat4 modelTrans = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
			//modelTrans = glm::translate(modelTrans, model->transform); 
			modelTrans = glm::translate(modelTrans, glm::vec3(0.0f, 0.0f, 0.0f));
			model->shader->setMat4("model", modelTrans);
			model->Draw();
		}
	}
}

