#pragma once

#include "Geometry.h"
#include "Mesh.h"

namespace Hazel
{
	class SphereMesh : public Mesh
	{
	public:
		class Interface
		{
			friend class SimpleGeometryModel;
		private:
			static Mesh* create(void* model) 
			{ 
				return new SphereMesh(model); 
			}
		};

	protected:
		SphereMesh(void*);
		
		// initializes all the buffer objects/arrays
		void setupMesh();

		virtual void drawAfterBindTextures() const;

		Sphere m_Sphere;
		unsigned int m_IndexCount = 0;
	};

}