#include "hzpch.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "Platform/Windows/OpenGLTexture.h"

#include "Material.h"

using namespace std;
namespace Hazel
{
    Material::~Material()
    {
        for(Texture* texture : m_Textures)
        {
            glDeleteTextures(1, &texture->id);
            if(texture == nullptr) delete texture;
        }
    }

    QuixelMaterial::QuixelMaterial(const std::vector<std::string>& folderPaths, QuixelObjectType type, Resolution resolution, unsigned int LODNumber)
    {
        vector<Texture*> texturesLoaded;
        for (const string& folderPath : folderPaths)
        {
            loadTexturesFromFolder(folderPath, texturesLoaded, type, resolution, LODNumber);
        }
    }

    QuixelMaterial::QuixelMaterial(const std::string& folderPath, QuixelObjectType type, Resolution resolution, unsigned int LODNumber)
    {
        vector<Texture*> texturesLoaded;
        loadTexturesFromFolder(folderPath, texturesLoaded, type, resolution, LODNumber);
    }


    bool QuixelMaterial::loadTexture(const std::string& filePath, std::vector<Texture*>& texturesLoaded, MyTextureType type)
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

            //NEED TO CONSIDER THIS FOR EVERY TEXTURE!!!
            //-------------------------------------------
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            //-------------------------------------------

            texture->type = type;
            texture->path = filePath;
            m_Textures.emplace_back(texture);
            texturesLoaded.emplace_back(texture);
            return true;
        }
        else
        {
            return false;
        }   
    }

    void QuixelMaterial::loadTexturesFromFolder(const string& folderPath, std::vector<Texture*>& texturesLoaded, QuixelObjectType type, Resolution resolution, unsigned int LODNumber)
    {
        if (type == QuixelObjectType::_3d)
        {
            //set the model's name
            m_Name = folderPath.substr(folderPath.find_last_of('_') + 1, folderPath.find_last_of('/') - folderPath.find_last_of('_') - 1);

            string textureFileName;
            textureFileName = folderPath + m_Name + '_' + ResolutionTypes[(unsigned int)resolution] + "_Normal" + "_LOD" + to_string(LODNumber) + ".jpg";
            loadTexture(textureFileName, texturesLoaded, myTextureType_NORMAL_CAMERA);
            textureFileName = folderPath + m_Name + '_' + ResolutionTypes[(unsigned int)resolution] + "_Roughness.jpg";
            loadTexture(textureFileName, texturesLoaded, myTextureType_DIFFUSE_ROUGHNESS);
            textureFileName = folderPath + m_Name + '_' + ResolutionTypes[(unsigned int)resolution] + "_AO.jpg";
            loadTexture(textureFileName, texturesLoaded, myTextureType_AMBIENT_OCCLUSION);
            textureFileName = folderPath + m_Name + '_' + ResolutionTypes[(unsigned int)resolution] + "_Displacement.jpg";
            loadTexture(textureFileName, texturesLoaded, myTextureType_DISPLACEMENT);
            if (0 <= LODNumber <= 3)
            {
                textureFileName = folderPath + m_Name + '_' + ResolutionTypes[(unsigned int)resolution] + "_Albedo.jpg";

            }
            else if (4 <= LODNumber <= 5)
            {
                textureFileName = folderPath + m_Name + '_' + ResolutionTypes[(unsigned int)resolution] + "_Albedo" + "_LOD" + to_string(LODNumber) + ".jpg";
            }
            loadTexture(textureFileName, texturesLoaded, myTextureType_BASE_COLOR);

        }
        else if (type == QuixelObjectType::_surface || type == QuixelObjectType::_3dplant)
        {
            //set the surface's name
            m_Name = folderPath.substr(folderPath.find_last_of('_') + 1, folderPath.find_last_of('/') - folderPath.find_last_of('_') - 1);

            string textureFileName;
            textureFileName = folderPath + m_Name + '_' + ResolutionTypes[(unsigned int)resolution] + "_Normal.jpg";
            loadTexture(textureFileName, texturesLoaded, myTextureType_NORMAL_CAMERA);
            textureFileName = folderPath + m_Name + '_' + ResolutionTypes[(unsigned int)resolution] + "_Roughness.jpg";
            loadTexture(textureFileName, texturesLoaded, myTextureType_DIFFUSE_ROUGHNESS);
            textureFileName = folderPath + m_Name + '_' + ResolutionTypes[(unsigned int)resolution] + "_AO.jpg";
            loadTexture(textureFileName, texturesLoaded, myTextureType_AMBIENT_OCCLUSION);
            textureFileName = folderPath + m_Name + '_' + ResolutionTypes[(unsigned int)resolution] + "_Displacement.jpg";
            loadTexture(textureFileName, texturesLoaded, myTextureType_DISPLACEMENT);
            textureFileName = folderPath + m_Name + '_' + ResolutionTypes[(unsigned int)resolution] + "_Albedo.jpg";
            loadTexture(textureFileName, texturesLoaded, myTextureType_BASE_COLOR);
            textureFileName = folderPath + m_Name + '_' + ResolutionTypes[(unsigned int)resolution] + "_Opacity.jpg";
            loadTexture(textureFileName, texturesLoaded, myTextureType_OPACITY);
        }     
    }
}

