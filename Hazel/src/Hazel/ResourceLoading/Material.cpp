#include "hzpch.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <assimp/material.h>

#include "Platform/Windows/OpenGLTexture.h"

#include "Material.h"

using namespace std;
namespace Hazel
{

    QuixelMaterial::QuixelMaterial(const std::string& folderPath, std::vector<Texture*>& texturesLoaded, QuixelObjectType type, unsigned int LODNumber)
    {
        loadTexturesFromFolder(folderPath, texturesLoaded, type, LODNumber);
    }

    QuixelMaterial::QuixelMaterial(const std::vector<std::string>& folderPaths, std::vector<Texture*>& texturesLoaded, QuixelObjectType type, unsigned int LODNumber)
    {
        for (const string& folderPath : folderPaths)
        {
            loadTexturesFromFolder(folderPath, texturesLoaded, type, LODNumber);
        }
    }

    bool QuixelMaterial::loadTexture(const std::string& filePath, std::vector<Texture*>& texturesLoaded, unsigned int type,  unsigned int LODNumber)
    {
        
        for (Texture* texture : texturesLoaded)
        {
            if (type == texture->type)//for time saving
            {
                if (filePath == texture->path)
                {
                    m_Textures.emplace_back(texture);
                    return true;
                }
            }
        }

        // tell stb_image.h to flip loaded texture's on the y-axis.
        stbi_set_flip_vertically_on_load(true);
        unsigned int id = textureFromFile(filePath);
        if (id != 0)
        {
            Texture* texture = new Texture;
            texture->id = id;
            glBindTexture(GL_TEXTURE_2D, texture->id);
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            texture->type = type;
            texture->path = filePath;
            texture->LODNumber = LODNumber;
            m_Textures.emplace_back(texture);
            texturesLoaded.emplace_back(texture);
            return true;
        }
        else
        {
            return false;
        }
        
    }

    void QuixelMaterial::loadTexturesFromFolder(const string& folderPath, std::vector<Texture*>& texturesLoaded, QuixelObjectType type, unsigned int LODNumber)
    {
        if (type == QuixelObjectType::_3d)
        {
            //set the model's name
            m_Name = folderPath.substr(folderPath.find_last_of('_') + 1, folderPath.find_last_of('/') - folderPath.find_last_of('_') - 1);

            string textureFileName;
            textureFileName = folderPath + m_Name + "_2K_Normal" + "_LOD" + to_string(LODNumber) + ".jpg";
            loadTexture(textureFileName, texturesLoaded, aiTextureType_NORMAL_CAMERA, LODNumber);
            textureFileName = folderPath + m_Name + "_2K_Roughness.jpg";
            loadTexture(textureFileName, texturesLoaded, aiTextureType_DIFFUSE_ROUGHNESS, LODNumber);
            textureFileName = folderPath + m_Name + "_2K_AO.jpg";
            loadTexture(textureFileName, texturesLoaded, aiTextureType_AMBIENT_OCCLUSION, LODNumber);
            textureFileName = folderPath + m_Name + "_2K_Displacement.jpg";
            loadTexture(textureFileName, texturesLoaded, aiTextureType_DISPLACEMENT, LODNumber);
            if (0 <= LODNumber <= 3)
            {
                textureFileName = folderPath + m_Name + "_2K_Albedo.jpg";

            }
            else if (4 <= LODNumber <= 5)
            {
                textureFileName = folderPath + m_Name + "_2K_Albedo" + "_LOD" + to_string(LODNumber) + ".jpg";
            }
            loadTexture(textureFileName, texturesLoaded, aiTextureType_BASE_COLOR, LODNumber);

        }
        else if (type == QuixelObjectType::_surface)
        {
            //set the surface's name
            m_Name = folderPath.substr(folderPath.find_last_of('_') + 1, folderPath.find_last_of('/') - folderPath.find_last_of('_') - 1);

            string textureFileName;
            textureFileName = folderPath + m_Name + "_2K_Normal.jpg";
            loadTexture(textureFileName, texturesLoaded, aiTextureType_NORMAL_CAMERA, LODNumber);
            textureFileName = folderPath + m_Name + "_2K_Roughness.jpg";
            loadTexture(textureFileName, texturesLoaded, aiTextureType_DIFFUSE_ROUGHNESS, LODNumber);
            textureFileName = folderPath + m_Name + "_2K_AO.jpg";
            loadTexture(textureFileName, texturesLoaded, aiTextureType_AMBIENT_OCCLUSION, LODNumber);
            textureFileName = folderPath + m_Name + "_2K_Displacement.jpg";
            loadTexture(textureFileName, texturesLoaded, aiTextureType_DISPLACEMENT, LODNumber);
            textureFileName = folderPath + m_Name + "_2K_Albedo.jpg";
            loadTexture(textureFileName, texturesLoaded, aiTextureType_BASE_COLOR, LODNumber);
            textureFileName = folderPath + m_Name + "_2K_Transmission.jpg";
            loadTexture(textureFileName, texturesLoaded, aiTextureType_OPACITY, LODNumber);
        }
        
    }
  

}

