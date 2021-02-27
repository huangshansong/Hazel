#pragma once
#include "hzpch.h"

#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"
#include "Hazel/ResourceLoading/model.h"

namespace Hazel 
{
	class HAZEL_API Actor
	{
	public :
		virtual void OnEvent(Event&);
		virtual void OnUpdate();

		std::vector<Model*> models;
	};
}