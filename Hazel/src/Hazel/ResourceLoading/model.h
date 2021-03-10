#pragma once 
#include "hzpch.h"

#include <glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"

namespace Hazel 
{

    class Model
    {
    public:
        class InterfaceUp
        {
            friend class Actor;
        private:
            static Model* create(void * actor, std::string const& filePath, bool gamma = false)
            { 
                Model* model = new Model(actor, gamma);
                model->readFile(filePath);
                return model; 
            }
            static void destroy(Model* model) { model->~Model(); }
            static void setName(Model* model, std::string& name) { model->m_Name = name; }
            static void setActor(Model* model, void* actor) { model->m_OfActor = actor; }
            static void draw(Model* model) { model->draw(); }
        };
        class InterfaceDown
        {
            friend class Mesh;
        private:
            static std::vector<Shader*>& getShadersLoaded(Model* model) { return model->m_ShadersLoaded; }
            static std::vector<Texture*>& getTexturesLoaded(Model* model) { return model->m_TexturesLoaded; }
        };
    public:
        const glm::vec3& getOfActorTransform();
        const std::string& getDirectory() { return m_Directory; }

    protected:
        Model(void* actor, bool gamma = false): m_OfActor(actor), m_GammaCorrection(gamma){}
        // expects a filepath to a 3D model.
        void readFile(std::string const& filePath);
        virtual ~Model() = default;       
    private:
        // draws the model, and thus all its meshes
        virtual void draw() const;
        
    protected:
        // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
        bool loadModel(std::string const& path);
        // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
        void processNode(aiNode* node, const aiScene* scene);
        Mesh* processMesh(aiMesh* mesh, const aiScene* scene);

        //for Model and its derived classes to access the mesh interface
        //-----------------------------------------------------------------------------------------------------------
        Mesh* createMesh(void* model, std::vector<Vertex>* vertices, std::vector<unsigned int>* indices, std::vector<Texture*>& textures,const std::string& name)
        {
            return Mesh::Interface::create(model, vertices, indices, textures, name);
        }
        void destroyMesh(Mesh* mesh) { Mesh::Interface::destroy(mesh); }
        void setMeshName(Mesh* mesh, std::string& name) { Mesh::Interface::setName(mesh, name); }
        void setMeshQuixelMaterial(Mesh* mesh, const std::string& folderPath, std::vector<Texture*>& texturesLoaded,
            QuixelObjectType type, unsigned int LODNumber = 0) 
        { 
            Mesh::Interface::setQuixelMaterial(mesh, folderPath, texturesLoaded, type, LODNumber);
        }
        void setMeshQuixelMaterial(Mesh* mesh, const std::vector<std::string>& folderPaths, std::vector<Texture*>& texturesLoaded,
            QuixelObjectType type, unsigned int LODNumber = 0)
        {
            Mesh::Interface::setQuixelMaterial(mesh, folderPaths, texturesLoaded, type, LODNumber);
        }
        void drawMesh(Mesh* mesh, glm::vec3& actorTransform) { Mesh::Interface::draw(mesh); }
        //-----------------------------------------------------------------------------------------------------------

        // model data 
        void* m_OfActor = nullptr;
        std::string m_Name = "Unnamed";
        std::string m_Directory;
        bool m_GammaCorrection;
        std::vector<Texture*> m_TexturesLoaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
        std::vector<Shader*> m_ShadersLoaded;
        std::vector<Mesh*> m_Meshes;

    };

}