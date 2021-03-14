#pragma once
#include "hzpch.h"

#include "Material.h"

#include "model.h"

namespace Hazel
{
	class HAZEL_API FromResourceModel : public Model
	{
	public:

		FromResourceModel(void* actor, std::string directory, bool gamma = false);

		const std::string& getDirectory() { return m_Directory; }

	protected:
		virtual void loadDirectory() = 0;

		// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
		bool loadModel(std::string const& filePath, unsigned int LODsNumber);

		// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
		void processNode(aiNode* node, const aiScene* scene, unsigned int LODsNumber);

		Mesh* processMesh(aiMesh* mesh, const aiScene* scene);

		virtual void processMaterial(Mesh* mesh, aiMaterial* material) = 0;

		virtual void processShader(Mesh* mesh);

		std::string m_Directory;
		bool m_GammaCorrection;

		std::vector<std::shared_ptr<Shader>> m_ShadersLoaded;
	};
}


