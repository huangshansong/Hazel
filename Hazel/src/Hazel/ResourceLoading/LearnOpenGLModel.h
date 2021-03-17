#pragma once

#include "FileSystem.h"
#include "FromResourceModel.h"

namespace Hazel
{
	class HAZEL_API LearnOpenGLModel : public FromResourceModel
	{
	public:
		LearnOpenGLModel(void* actor, std::string directory, FileSuffix, bool gamma = false);

	protected:
		virtual void loadDirectory() override;

		virtual void processMaterial(Mesh* mesh, aiMaterial* material) override;

		std::vector<Texture*> m_TexturesLoaded;

		FileSuffix m_FileSuffix;
	};

}

