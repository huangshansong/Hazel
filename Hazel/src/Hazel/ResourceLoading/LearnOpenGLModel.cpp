#include "hzpch.h"

#include <Glad/glad.h> 
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Hazel/Core.h"
#include "Hazel/Log.h"
#include "Platform/Windows/OpenGLTexture.h"

#include "FileSystem.h"
#include "Material.h"
#include "Hazel/Actors/Actor.h"
#include "mesh.h"
#include "model.h"
#include "FromResourceModel.h"

#include "LearnOpenGLModel.h"

using namespace std;
namespace Hazel
{
	LearnOpenGLModel::LearnOpenGLModel(void* actor, std::string directory, bool gamma)
		: FromResourceModel(actor, directory, gamma)
	{
        loadDirectory();
	}

	void LearnOpenGLModel::loadDirectory()
	{
		m_Name = m_Directory.substr(m_Directory.find_last_of('/', m_Directory.size() - 2) + 1, m_Directory.find_last_of('/') - m_Directory.find_last_of('/', m_Directory.size() - 2) - 1);
		string filePath = m_Directory + m_Name + ".obj";
		loadModel(filePath, 0);
	}

	void LearnOpenGLModel::processMaterial(Mesh* mesh, aiMaterial* material)
	{
        vector<Texture*> textures;
        for (unsigned int type = 1; type <= sizeof(textureTypes) / sizeof(char*) - 2; type++)
        {
            for (unsigned int i = 0; i < material->GetTextureCount(aiTextureType(type)); i++)
            {
                aiString str;
                material->GetTexture(aiTextureType(type), i, &str);
                string path = m_Directory + str.C_Str();
                // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
                bool skip = false;
                for (unsigned int j = 0; j < m_TexturesLoaded.size(); j++)
                {
                    if (m_TexturesLoaded[j]->path == path)
                    {
                        textures.emplace_back(m_TexturesLoaded[j]);
                        skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                        break;
                    }
                }
                if (!skip)
                {   // if texture hasn't been loaded already, load it
                    Texture* texture = new Texture;
                    texture->id = textureFromFile(path);
                    glBindTexture(GL_TEXTURE_2D, texture->id);
                    glGenerateMipmap(GL_TEXTURE_2D);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    texture->type = (MyTextureType)type;
                    texture->path = path;
                    textures.emplace_back(texture);
                    m_TexturesLoaded.emplace_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
                }
            }
        }
        OrdinaryMaterial* myMaterial = new OrdinaryMaterial(textures);
        mesh->setMaterial(shared_ptr<Material>(myMaterial));

	}
}
