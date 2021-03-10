#pragma once
#include "hzpch.h"

#include <glm.hpp>

#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"
#include "Hazel/ResourceLoading/model.h"
#include "Hazel/ResourceLoading/landscape.h"
#include "Hazel/ResourceLoading/SimpleGeometryModel.h"

namespace Hazel 
{
	//add every new default actor's name here
	const char* const defaultActorNames[] =
	{
		"Backpack",
		"Landscape",
		"Sphere"
	};
	class HAZEL_API Actor
	{
	public:
		class Interface
		{
			friend class Level;
		private:
			static Actor* create(std::string name = "Unnamed",glm::vec3 transform = glm::vec3(0.0f, 0.0f, 0.0f)) { return new Actor(name, transform); }
			static void destroy(Actor* actor) { actor->~Actor(); }
			
			static void setTransform(Actor* actor, glm::vec3& transform) { actor->m_Transform = transform; }
			static void setName(Actor* actor, std::string& name) { actor->m_Name = name; }

			static void onEvent(Actor* actor, Event& event) { actor->onEvent(event); }
			static void onUpdate(Actor* actor) { actor->onUpdate(); }
			static void onRender(Actor* actor) { actor->onRender(); }
		};
	public:
		const glm::vec3& getTransform() { return m_Transform; }
		const std::string& getName() { return m_Name; }

	private:
		virtual void onEvent(Event&);
		virtual void onUpdate();
		virtual void onRender();

	protected:
		Actor(std::string name = "Unnamed", glm::vec3 transform = glm::vec3(0.0f, 0.0f, 0.0f));
		virtual ~Actor() = default;

		//for Actor and its derived classes to access the model interface
		//-----------------------------------------------------------------------------------------------------------
		Model* createModel(std::string const& path, bool gamma = false) { return Model::InterfaceUp::create(this, path, gamma);}
		Model* createLandscapeModel(unsigned int width = 1000, unsigned int length = 1000){return LandscapeModel::Interface::create(this, width, length);}
		Model* createSimpleGeometryModel(SimpleGeometryType type){return SimpleGeometryModel::Interface::create(this, type);}
		void destroyModel(Model* model) { Model::InterfaceUp::destroy(model); }
		void setName(Model* model, std::string& name) { Model::InterfaceUp::setName(model, name); }
		void drawModel(Model* model) { Model::InterfaceUp::draw(model); }
		//-----------------------------------------------------------------------------------------------------------

		

	protected:
		
		std::string m_Name;
		Model* m_Model = nullptr;
		glm::vec3 m_Transform;

	};
}