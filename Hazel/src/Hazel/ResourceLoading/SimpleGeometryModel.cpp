#include "hzpch.h"

#include "SimpleGeometryMesh.h"
#include "SimpleGeometryModel.h"

using namespace std;
namespace Hazel
{
	SimpleGeometryModel::SimpleGeometryModel(void* actor, SimpleGeometryType type)
		: Model(actor)
	{
		if (type == SimpleGeometryType::Sphere)
		{
			m_Directory = "resources/simpleGeometry/";
			m_Meshes.emplace_back(SphereMesh::Interface::create(this));
		}
	}
}