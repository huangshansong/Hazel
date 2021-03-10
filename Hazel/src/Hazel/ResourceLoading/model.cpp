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

using namespace std;
namespace Hazel
{
    const glm::vec3& Model::getOfActorTransform()
    {
         return ((Actor*)m_OfActor)->getTransform(); 
    }
    void Model::readFile(string const& filePath)
    {
        // retrieve the directory path of the filepath
        m_Directory = filePath.substr(0, filePath.find_last_of('/') + 1);

        loadModel(filePath);
    }

    // draws the model, and thus all its meshes
    void Model::draw() const
    {
        for (unsigned int i = 0; i < m_Meshes.size(); i++)
        {
            Mesh::Interface::draw(m_Meshes[i]);
        }
        //HZ_CORE_INFO(m_Meshes.size());
    }

    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    bool Model::loadModel(string const& filePath)
    {
        // read file via ASSIMP
        Assimp::Importer importer;
        //const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        const aiScene* scene = importer.ReadFile(FileSystem::getPath(filePath), 0);
        // check for errors
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            HZ_CORE_INFO(importer.GetErrorString());
            return false;
        }
        // process ASSIMP's root node recursively
        processNode(scene->mRootNode, scene);

        HZ_CORE_INFO("model loaded.");
        return true;
    }

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void Model::processNode(aiNode* node, const aiScene* scene)
    {
        // process each mesh located at the current node
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            m_Meshes.emplace_back(processMesh(mesh, scene));
            HZ_CORE_INFO("mesh loaded.");
            
        }
        HZ_CORE_INFO("meshs loaded.");
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }

    }

    Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene)
    {
        // data to fill
        vector<Vertex>* vertices = new vector<Vertex>;
        vector<unsigned int>* indices = new vector<unsigned int>;
        vector<Texture*> textures;

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

        // process materials
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

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
                        texture->type = type;
                        texture->path = path;
                        textures.emplace_back(texture);
                        m_TexturesLoaded.emplace_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
                    }
                }
            }
        } 
        // return a mesh object created from the extracted mesh data
        return createMesh(this, vertices, indices, textures, mesh->mName.C_Str());
    }

}