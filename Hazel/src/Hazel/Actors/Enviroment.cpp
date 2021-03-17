#include "hzpch.h"

#include "Enviroment.h"

using namespace std;
namespace Hazel
{
	Enviroment::Enviroment(void* level, std::string name, glm::vec3 transform)
		: Actor(level, name, transform)
	{
	}
	void Enviroment::onRender() const
	{
		// set depth function to less than AND equal for skybox depth trick.
		glDepthFunc(GL_LEQUAL);
		// enable seamless cubemap sampling for lower mip levels in the pre-filter map.
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		m_Model->draw();
	}
}
