#pragma once

#include "FromResourceModel.h"

namespace Hazel
{
	class HAZEL_API LearnOpenGLModel : public FromResourceModel
	{
	public:
		LearnOpenGLModel(void* actor, std::string directory, bool gamma = false);

	protected:
		virtual void loadDirectory() override;

		virtual void processMaterial(Mesh* mesh, aiMaterial* material) override;

		//just set Universal shader for LearnOpenGLModel
		virtual void processShader(Mesh* mesh) override {};

		std::vector<Texture*> m_TexturesLoaded;
	};

}

