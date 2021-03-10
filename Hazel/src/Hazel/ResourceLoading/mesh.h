#pragma once
#include "hzpch.h"

#include <glm.hpp>

#include "QuixelFile.h"
#include "Material.h"
#include "shader.h"

namespace Hazel 
{

    struct Vertex 
    {
        // position
        glm::vec3 Position;
        // normal
        glm::vec3 Normal;
        // texCoords
        glm::vec2 TexCoords;
        // tangent
        glm::vec3 Tangent;
        // bitangent
        glm::vec3 Bitangent;
    };

    class Mesh 
    {
    public:
        class Interface
        {
            friend class Model;
            friend class QuixelModel;
        private:
            static Mesh* create(void* model, std::vector<Vertex>* vertices, std::vector<unsigned int>* indices, std::vector<Texture*>& textures, const std::string& name)
            { 
                Mesh* mesh = new Mesh(model);
                mesh->init(vertices, indices, textures, name);
                return mesh; 
            }
            static void destroy(Mesh* mesh) { mesh->~Mesh(); }

            static void setModel(Mesh* mesh, void* model) { mesh->m_OfModel = model; }
            static void setName(Mesh* mesh, std::string& name) { mesh->m_Name = name; }
            static void setQuixelMaterial(Mesh* mesh, const std::string& folderPath, std::vector<Texture*>& texturesLoaded,
                QuixelObjectType type, unsigned int LODNumber = 0)
            { 
                mesh->setQuixelMaterial(folderPath, texturesLoaded, type, LODNumber); 
            }
            static void setQuixelMaterial(Mesh* mesh, const std::vector<std::string>& folderPaths, std::vector<Texture*>& texturesLoaded,
                QuixelObjectType type, unsigned int LODNumber = 0)
            { 
                mesh->setQuixelMaterial(folderPaths, texturesLoaded, type, LODNumber); 
            }

            static void draw(Mesh* mesh) { mesh->draw(); }
        };
    protected:
        void init(std::vector<Vertex>* vertices, std::vector<unsigned int>* indices, std::vector<Texture*>& textures, const std::string& name);
        virtual ~Mesh() = default;

    private:
        // render the mesh
        virtual void draw() const;

    protected:
        Mesh(void* model): m_OfModel(model) {}

        //override this if the mesh is without indices!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        virtual void drawAfterBindTextures() const;
        //override this if the mesh is without indices!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        // initializes all the buffer objects/arrays
        void bindBufferAndAttribute(std::vector<Vertex>&, std::vector<unsigned int>&);
        void bindTexture() const;
        void bindShader();
        void setShaderUniforms() const;

        //for Mesh and its derived classes to access the material interface
        //-----------------------------------------------------------------------------------------------------------
        Material* createMaterial(std::vector<Texture*>& textures) { return Material::Interface::create(textures); }
        void destroyMaterial(Material* material) { Material::Interface::destroy(material); }
        const std::vector<Texture*>& getMaterialTextures(Material* material) const { return Material::Interface::getTextures(material); }
        
        void setQuixelMaterial(const std::string& folderPath, std::vector<Texture*>& texturesLoaded,
            QuixelObjectType type, unsigned int LODNumber = 0)
        {
            m_Material = QuixelMaterial::Interface::create(folderPath, texturesLoaded, type, LODNumber);
        }
        void setQuixelMaterial(const std::vector<std::string>& folderPaths, std::vector<Texture*>& texturesLoaded,
            QuixelObjectType type, unsigned int LODNumber = 0)
        {
            m_Material = QuixelMaterial::Interface::create(folderPaths, texturesLoaded, type, LODNumber);
        }
        //-----------------------------------------------------------------------------------------------------------
        //for Mesh and its derived classes to access the model interface
        //-----------------------------------------------------------------------------------------------------------
        std::vector<Texture*>& getModelTexturesLoaded();
        std::vector<Shader*>& getModelShadersLoaded();
        //-----------------------------------------------------------------------------------------------------------


        // render data 
        unsigned int m_VAO;
        unsigned int m_VBO, m_EBO;
        Shader* m_Shader;
        // mesh data
        void* m_OfModel = nullptr;
        std::vector<Vertex>* m_Vertices;
        std::vector<unsigned int>* m_Indices;
        Material* m_Material;
        std::string m_Name = "Unnamed";

        //game data

    };

}

