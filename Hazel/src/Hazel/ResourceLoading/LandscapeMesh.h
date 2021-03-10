#pragma once

#include "Mesh.h"
#include "Hazel/ResourceLoading/landscape.h"


namespace Hazel
{
	class LandscapeMesh : public Mesh
	{
	public:
		
		LandscapeMesh(LandscapeModel* landscape);
		
	protected:


		// initializes all the buffer objects/arrays
		void setupMesh(LandscapeModel* landscape);

		virtual void drawAfterBindTextures() const;

		
		unsigned int m_IndexCount = 0;

		
	};

}