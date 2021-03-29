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
#include "FromResourceMesh.h"

using namespace std;
namespace Hazel
{
	FromResourceModel::FromResourceModel(void* actor, std::string directory, bool gamma)
		: Model(actor), m_Directory(directory), m_GammaCorrection(gamma)
	{
	}

    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    bool FromResourceModel::loadModel(string const& filePath, unsigned int LODsNumber)
    {
        // read file via ASSIMP
        Assimp::Importer importer;
        //const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        const aiScene* scene = importer.ReadFile(FileSystem::getPath(filePath), aiProcess_Triangulate | aiProcess_GenSmoothNormals);
        // check for errors
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            HZ_CORE_INFO(importer.GetErrorString());
            return false;
        }

        vector<shared_ptr<Mesh>> temp;
        m_LODs.emplace_back(temp);
        // process ASSIMP's root node recursively
        processNode(scene->mRootNode, scene, LODsNumber);

        HZ_CORE_INFO("model loaded.");
        return true;
    }

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void FromResourceModel::processNode(aiNode* node, const aiScene* scene, unsigned int LODsNumber)
    {
        // process each mesh located at the current node
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            HZ_CORE_INFO(LODsNumber);
            m_LODs[LODsNumber].emplace_back(shared_ptr<Mesh>(processMesh(mesh, scene)));
            HZ_CORE_INFO("mesh loaded.");

        }
        HZ_CORE_INFO("meshs loaded.");
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene, LODsNumber);
        }

    }

    Mesh* FromResourceModel::processMesh(aiMesh* mesh, const aiScene* scene)
    {
        // data to fill
        vector<Vertex>* vertices = new vector<Vertex>;
        vector<unsigned int>* indices = new vector<unsigned int>;
        
        // walk through each of the mesh's vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            // texture coordinates
            if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
                /*
                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
                */
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            (*vertices).push_back(vertex);
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                (*indices).push_back(face.mIndices[j]);
        }

        Mesh* myMesh = new FromResourceMesh(this, vertices, indices, mesh->mName.C_Str());

        // process material
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        processMaterial(myMesh, material);

        // process shader
        processShader(myMesh);

        // return a mesh object created from the extracted mesh data
        return myMesh;
    }
    void FromResourceModel::processShader(Mesh* mesh)
    {
        bool meshHasUniqueShader = false;
        string shaderPath;
        for (int i = 0; i <= 1; i++)
        {
            if (i == 0)
            {
                shaderPath = m_Directory + m_Name;
            }
            else
            {
                shaderPath = m_Directory + "Universal";
            }
            bool skip = false;
            for (shared_ptr<Shader> shader : m_ShadersLoaded)
            {
                if (shader->getVertexShaderPath() == shaderPath + ".vs")
                {
                    mesh->setShader(shader);
                    skip = true;
                    break;
                }
            }
            if (!skip)
            {
                string vertexShaderPath = shaderPath + ".vs";
                string fragmentShaderPath = shaderPath + ".fs";
                string geometryShaderPath;
                Shader* shader = new Shader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
                if (shader->getID() != 0)
                {
                    mesh->setShader(shared_ptr<Shader>(shader));
                    break;//if the mesh has its unique shader, no need to load the Universal shader 
                }
            }
        }
    }
}
