#pragma once
#include "hzpch.h"

#include "Hazel/Actors/camera.h"
#include "Hazel/Actors/Actor.h"
#include "Hazel/Events/Event.h"

namespace Hazel 
{
	class Level
	{
		friend class LevelLayer;
	public:
		Camera* getCamera() { return m_CurrentCamera; }
	protected:
		
		Actor* createActor(std::string name = "Unnamed", glm::vec3 transform = glm::vec3(0.0f, 0.0f, 0.0f)) { return Actor::Interface::create(name, transform); }
		void destroyActor(Actor* actor) { Actor::Interface::destroy(actor); }
		void setActorTransform(Actor* actor, glm::vec3 transform) { Actor::Interface::setTransform(actor, transform); }
		void setActorName(Actor* actor, std::string& name) { Actor::Interface::setName(actor, name); }
		void callActorOnRender(Actor* actor) { Actor::Interface::onRender(actor); }
		void callActorOnUpdate(Actor* actor) { Actor::Interface::onUpdate(actor); }
		void callActorOnEvent(Actor* actor, Event& event) { Actor::Interface::onEvent(actor, event); }
		
		Actor* createCamera(glm::vec3& transform = glm::vec3(0.0f, 0.0f, 0.0f)) { return Camera::Interface::create(transform); }

		void init();
		void onRender();
		void onUpdate();
		void onEvent(Event&);

		void gen_ItemLocation();

		std::vector<Actor*> m_Actors;
		Camera* m_CurrentCamera;
		static bool m_FirstLevel;
	};

}

