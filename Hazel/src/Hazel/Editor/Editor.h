#pragma once
#include "hzpch.h"

#include "Hazel/Window.h"
#include "Hazel/Levels/Level.h"
#include "Hazel/Actors/Actor.h"


#include "Hazel/HObject.h"

namespace Hazel
{
	class HAZEL_API Editor : public HObject
	{
	public:
		Editor(void* application);

		virtual ~Editor() = default;
		
		virtual void pollEditions();

		//SCRIPTS
		//----------------------------------------------------

		//call this in GUI.init()
		void canDiplayTest(Window* window);

		void addDefaultMap(Level* level);

		void addDefaultActor(Level* level, DefaultActor actor);
		//----------------------------------------------------

		typedef void (*EditorCreateCallback)();//functions here contains only EMPLACE_BACK codes
		typedef void (*EditorSetCallback)();//functions here contains only SET codes
		typedef void (*EditorDestroyCallback)();//functions here contains only ERASE codes
		void addCreateCallback(EditorCreateCallback& callback) { createCallbacks.emplace_back(callback); }
		void addSetCallback(EditorCreateCallback& callback) { setCallbacks.emplace_back(callback); }
		void addDestroyCallback(EditorCreateCallback& callback) { destroyCallbacks.emplace_back(callback); }
	protected:
		std::vector<EditorCreateCallback> createCallbacks;
		std::vector<EditorSetCallback> setCallbacks;
		std::vector<EditorDestroyCallback> destroyCallbacks;

		void* m_OfApplication;
	};
}


