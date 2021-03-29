#include "hzpch.h"

#include "Hazel/ResourceLoading/FileSystem.h"
#include "Hazel/ResourceLoading/QuixelFile.h"

#include "PlantsLevel.h"

using namespace std;
namespace Hazel
{
	void PlantsLevel::onRender()
	{
		glDisable(GL_CULL_FACE);
		Level::onRender();
	}
	void PlantsLevel::gen_PlantsLocation(LandscapeModel* landscape)
	{
	}
}
