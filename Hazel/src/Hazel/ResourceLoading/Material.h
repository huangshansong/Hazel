#pragma once
#include "hzpch.h"

#include "QuixelFile.h"

namespace Hazel
{
    struct Texture {
        unsigned int id;
        unsigned int type;
        std::string path;
        unsigned int LODNumber = 0;
    };

    //copied from Assimp
    const char* const textureTypes[] =
    {
        "none",
        "diffuseMap",
        "specularMap",
        "ambientMap",
        "emmisiveMap",
        "heightMap",
        "nomalsMap",
        "shininessMap",
        "opacityMap",
        "displacementMap",
        "lightmapMap",
        "reflectionMap",
        "albedoMap",
        "normalMap",
        "emmisionMap",
        "metallicMap",
        "roughnessMap",
        "aoMap",
        "unknown"
    };

    

    class Material
    {
    public:
        class Interface
        {
            friend class Mesh;
        private:
            static Material* create(std::vector<Texture*>& textures) { return new Material(textures); }
            static void destroy(Material* material) { material->~Material(); }
            
            static const std::vector<Texture*>& getTextures(const Material* material) { return material->m_Textures; }
        };
    protected:
        Material() {}
        Material(std::vector<Texture*>& textures) : m_Textures(textures) {}
        virtual ~Material() = default;

    protected:
        
        std::vector<Texture*> m_Textures;
        std::string m_Name;
    };

    class QuixelMaterial : public Material
    {
    public:
        class Interface
        {
            friend class Mesh;
            friend class LandscapeMesh;
        private:
            static Material* create(const std::string& folderPath, std::vector<Texture*>& texturesLoaded, 
                QuixelObjectType type, unsigned int LODNumber)
                { return new QuixelMaterial(folderPath, texturesLoaded, type, LODNumber); }
            static Material* create(const std::vector<std::string>& folderPaths, std::vector<Texture*>& texturesLoaded, 
                QuixelObjectType type, unsigned int LODNumber = 0)
                { return new QuixelMaterial(folderPaths, texturesLoaded, type, LODNumber); }

        };
    protected:
        QuixelMaterial(const std::string&, std::vector<Texture*>&, QuixelObjectType, unsigned int);
        QuixelMaterial(const std::vector<std::string>&, std::vector<Texture*>&, QuixelObjectType, unsigned int);

        bool loadTexture(const std::string&, std::vector<Texture*>&, unsigned int, unsigned int);
        //currentlly it can only load the 2K textures
        void loadTexturesFromFolder(const std::string&, std::vector<Texture*>&, QuixelObjectType, unsigned int);

    };
    
}