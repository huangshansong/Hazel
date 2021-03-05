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
#include "Hazel/Actors/landscape.h"

#include "mesh.h"
#include "shader.h"
#include "model.h"

using namespace std;
namespace Hazel
{
	extern unsigned int TextureFromFile(const string& path, bool gamma = false);

    // constructor, expects a filepath to a 3D model.
    Model::Model(string const& path,  bool fromQuixel, bool gamma)
        : m_FromQuixel(fromQuixel), m_GammaCorrection(gamma)
    {
        loadModel(path);
    }
    // constructor, expects a heightMap, for landscape
    Model::Model(Landscape* landscape, bool gamma)//!!!should add Material and texCords later!!!
        : m_GammaCorrection(gamma)
    {
        loadModel(landscape);
    }

    // draws the model, and thus all its meshes
    void Model::draw()
    {
        for (unsigned int i = 0; i < m_Meshes.size(); i++)
        {
            m_Meshes[i]->draw();
        }
        //HZ_CORE_INFO(m_Meshes.size());
    }

    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void Model::loadModel(string const& path)
    {
        // read file via ASSIMP
        Assimp::Importer importer;
        //const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        const aiScene* scene = importer.ReadFile(path, 0);
        // check for errors
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }
        // retrieve the directory path of the filepath
        m_Directory = path.substr(0, path.find_last_of('/'));

        //set the model's name, only for the Quxel's model
        if (m_FromQuixel)
        {
            m_ModelName = path.substr(path.find_last_of('/') + 1, path.find_last_of('_') - path.find_last_of('/') - 1);
        }

        // process ASSIMP's root node recursively
        processNode(scene->mRootNode, scene);

        HZ_CORE_INFO("model loaded.");
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
        vector<Texture>* textures = new vector<Texture>;

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
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        vector<Texture> albedo = loadMaterialTextures(material, aiTextureType_BASE_COLOR, "texture_albedo");
        (*textures).insert((*textures).end(), albedo.begin(), albedo.end());
        vector<Texture> normal = loadMaterialTextures(material, aiTextureType_NORMAL_CAMERA, "texture_normal");
        (*textures).insert((*textures).end(), normal.begin(), normal.end());
        std::vector<Texture> emission = loadMaterialTextures(material, aiTextureType_EMISSION_COLOR, "texture_emission");
        (*textures).insert((*textures).end(), emission.begin(), emission.end());
        std::vector<Texture> metallic = loadMaterialTextures(material, aiTextureType_METALNESS, "texture_metallic");
        (*textures).insert((*textures).end(), metallic.begin(), metallic.end());
        std::vector<Texture> roughness = loadMaterialTextures(material, aiTextureType_DIFFUSE_ROUGHNESS, "texture_roughness");
        (*textures).insert((*textures).end(), roughness.begin(), roughness.end());
        std::vector<Texture> ao = loadMaterialTextures(material, aiTextureType_AMBIENT_OCCLUSION, "texture_ao");
        (*textures).insert((*textures).end(), ao.begin(), ao.end());
        std::vector<Texture> displacement = loadMaterialTextures(material, aiTextureType_DISPLACEMENT, "texture_displacement");
        (*textures).insert((*textures).end(), displacement.begin(), displacement.end());

        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN
        // 1. diffuse maps
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        (*textures).insert((*textures).end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        (*textures).insert((*textures).end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        (*textures).insert((*textures).end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<Texture> ambientLightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_ambientLight");
        (*textures).insert((*textures).end(), ambientLightMaps.begin(), ambientLightMaps.end());

        // return a mesh object created from the extracted mesh data
        return new Mesh(vertices, indices, textures);
    }

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
    {
        vector<Texture> textures;
        bool NoTextureInModelFile = true;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            string path = this->m_Directory + '/' + str.C_Str();
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = false;
            for (unsigned int j = 0; j < m_Textures_Loaded.size(); j++)
            {
                if (m_Textures_Loaded[j].path == path)
                {
                    textures.push_back(m_Textures_Loaded[j]);
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if (!skip)
            {   // if texture hasn't been loaded already, load it
                Texture texture;
                texture.id = TextureFromFile(path);
                glBindTexture(GL_TEXTURE_2D, texture.id);
                glGenerateMipmap(GL_TEXTURE_2D);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                texture.type = typeName;
                texture.path = path;
                textures.push_back(texture);
                m_Textures_Loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            }

            NoTextureInModelFile = false;
        }
        if (NoTextureInModelFile)
        {
            // this is for the Quixel's models
            if (m_FromQuixel)
            {
                string textureFileName;
                bool textureNotConsidered = false;
                switch (type)
                {
                case aiTextureType_BASE_COLOR:
                {
                    textureFileName = m_ModelName + "_2K_Albedo.jpg";
                    break;
                }
                case aiTextureType_NORMAL_CAMERA:
                {
                    textureFileName = m_ModelName + "_2K_Normal_LOD0.jpg";
                    break;
                }
                case aiTextureType_DIFFUSE_ROUGHNESS:
                {
                    textureFileName = m_ModelName + "_2K_Roughness.jpg";
                    break;
                }
                case aiTextureType_AMBIENT_OCCLUSION:
                {
                    textureFileName = m_ModelName + "_2K_AO.jpg";
                    break;
                }
                case aiTextureType_DISPLACEMENT:
                {
                    textureFileName = m_ModelName + "_2K_Displacement.jpg";
                    break;
                }
                default:
                {
                    textureNotConsidered = true;
                }
                }
                if (!textureNotConsidered)
                {
                    bool skip = false;
                    string path = this->m_Directory + '/' + textureFileName.c_str();
                    for (unsigned int j = 0; j < m_Textures_Loaded.size(); j++)
                    {
                        if (m_Textures_Loaded[j].path == path)
                        {
                            textures.push_back(m_Textures_Loaded[j]);
                            skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                            break;
                        }
                    }
                    if (!skip)
                    {   // if texture hasn't been loaded already, load it
                        Texture texture;
                        texture.id = TextureFromFile(path);
                        glBindTexture(GL_TEXTURE_2D, texture.id);
                        glGenerateMipmap(GL_TEXTURE_2D);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                        texture.type = typeName;
                        texture.path = path;
                        textures.push_back(texture);
                        m_Textures_Loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
                        //HZ_CORE_INFO(typeName.c_str());
                    }
                }
            }

        }
        HZ_CORE_INFO("textures loaded.");
        return textures;
    }

    vector<Texture> Model::loadMaterialTextures(Landscape* landscape, aiTextureType type, string typeName)
    {
        vector<Texture> textures;
        for (unsigned int i = 0; i < landscape->getMaterialsPaths().size(); i++)
        {

            string path = landscape->getMaterialsPaths()[i];
            string textureFileName;
            string surfaceName = path.substr(path.find_last_of('_') + 1, path.find_last_of('/') - path.find_last_of('_') - 1);
            bool textureNotConsidered = false;
            switch (type)
            {
            case aiTextureType_BASE_COLOR:
            {
                textureFileName = surfaceName + "_2K_Albedo.jpg";
                break;
            }
            case aiTextureType_NORMAL_CAMERA:
            {
                textureFileName = surfaceName + "_2K_Normal.jpg";
                break;
            }
            case aiTextureType_DIFFUSE_ROUGHNESS:
            {
                textureFileName = surfaceName + "_2K_Roughness.jpg";
                break;
            }
            case aiTextureType_AMBIENT_OCCLUSION:
            {
                textureFileName = surfaceName + "_2K_AO.jpg";
                break;
            }
            case aiTextureType_DISPLACEMENT:
            {
                textureFileName = surfaceName + "_2K_Displacement.jpg";
                break;
            }
            default:
            {
                textureNotConsidered = true;
            }
            }
            if (!textureNotConsidered)
            {
                bool skip = false;
                path = path + textureFileName;
                for (unsigned int j = 0; j < m_Textures_Loaded.size(); j++)
                {
                    if (m_Textures_Loaded[j].path == path)
                    {
                        textures.push_back(m_Textures_Loaded[j]);
                        skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                        break;
                    }
                }
                if (!skip)
                {   // if texture hasn't been loaded already, load it
                    Texture texture;
                    texture.id = TextureFromFile(path);
                    glBindTexture(GL_TEXTURE_2D, texture.id);
                    glGenerateMipmap(GL_TEXTURE_2D);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    texture.type = typeName;
                    texture.path = path;
                    textures.push_back(texture);
                    m_Textures_Loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
                    //HZ_CORE_INFO(typeName.c_str());
                }
            }
        }
        HZ_CORE_INFO("textures loaded.");
        return textures;
    }

    // loads a model from heightMap and stores the resulting meshes in the meshes vector.
    void Model::loadModel(Landscape* landscape)
    {
        // data to fill
        vector<Vertex>* vertices = new vector<Vertex>;
        vector<unsigned int>* indices = new vector<unsigned int>;
        vector<Texture>* textures = new vector<Texture>;

        unsigned int Width = landscape->getWidth();
        unsigned int Length = landscape->getLength();

        for (unsigned int i = 0; i < Width - 1; i++)
        {
            for (unsigned int j = 0; j < Length - 1; j++)
            {
                Vertex vertex;

                glm::vec3 vectorLT;
                vectorLT.x = (float)i;
                vectorLT.z = (float)j;
                vectorLT.y = landscape->getHeight(i, j);

                glm::vec3 vectorRT;
                vectorRT.x = (float)i;
                vectorRT.z = (float)(j + 1);
                vectorRT.y = landscape->getHeight(i, j + 1);

                glm::vec3 vectorLB;
                vectorLB.x = (float)(i + 1);
                vectorLB.z = (float)j;
                vectorLB.y = landscape->getHeight(i + 1, j);

                glm::vec3 vectorRB;
                vectorRB.x = (float)(i + 1);
                vectorRB.z = (float)(j + 1);
                vectorRB.y = landscape->getHeight(i + 1, j + 1);

                glm::vec3 normalofLTtriangel = glm::normalize(glm::cross(vectorLT - vectorRT, vectorLB - vectorLT));
                glm::vec3 normalofRBtriangel = glm::normalize(glm::cross(vectorRB - vectorLB, vectorRT - vectorRB));

                vertex.Position = vectorLT / 10.0f;
                vertex.Normal = normalofLTtriangel;
                vertex.TexCoords = glm::vec2(vertex.Position.x / 10.0f, vertex.Position.z / 10.0f);
                //vertex.TexCoords = glm::vec2(0.5, 0.5);
                (*vertices).emplace_back(vertex);
                vertex.Position = vectorRT / 10.0f;
                vertex.Normal = normalofLTtriangel;
                vertex.TexCoords = glm::vec2(vertex.Position.x / 10.0f, vertex.Position.z / 10.0f);
                //vertex.TexCoords = glm::vec2(0.5, 0.5);
                (*vertices).emplace_back(vertex);
                vertex.Position = vectorLB / 10.0f;
                vertex.Normal = normalofLTtriangel;
                vertex.TexCoords = glm::vec2(vertex.Position.x / 10.0f, vertex.Position.z / 10.0f);
                //vertex.TexCoords = glm::vec2(0.5, 0.5);
                (*vertices).emplace_back(vertex);
                vertex.Position = vectorLB / 10.0f;
                vertex.Normal = normalofRBtriangel;
                vertex.TexCoords = glm::vec2(vertex.Position.x / 10.0f, vertex.Position.z / 10.0f);
                //vertex.TexCoords = glm::vec2(0.5, 0.5);
                (*vertices).emplace_back(vertex);
                vertex.Position = vectorRT / 10.0f;
                vertex.Normal = normalofRBtriangel;
                vertex.TexCoords = glm::vec2(vertex.Position.x / 10.0f, vertex.Position.z / 10.0f);
                //vertex.TexCoords = glm::vec2(0.5, 0.5);
                (*vertices).emplace_back(vertex);
                vertex.Position = vectorRB / 10.0f;
                vertex.Normal = normalofRBtriangel;
                vertex.TexCoords = glm::vec2(vertex.Position.x / 10.0f, vertex.Position.z / 10.0f);
                //vertex.TexCoords = glm::vec2(0.5, 0.5);
                (*vertices).emplace_back(vertex);

            }
        }

        //load textures
        {
            vector<Texture> albedo = loadMaterialTextures(landscape, aiTextureType_BASE_COLOR, "texture_albedo");
            (*textures).insert((*textures).end(), albedo.begin(), albedo.end());
            vector<Texture> normal = loadMaterialTextures(landscape, aiTextureType_NORMAL_CAMERA, "texture_normal");
            (*textures).insert((*textures).end(), normal.begin(), normal.end());
            std::vector<Texture> emission = loadMaterialTextures(landscape, aiTextureType_EMISSION_COLOR, "texture_emission");
            (*textures).insert((*textures).end(), emission.begin(), emission.end());
            std::vector<Texture> metallic = loadMaterialTextures(landscape, aiTextureType_METALNESS, "texture_metallic");
            (*textures).insert((*textures).end(), metallic.begin(), metallic.end());
            std::vector<Texture> roughness = loadMaterialTextures(landscape, aiTextureType_DIFFUSE_ROUGHNESS, "texture_roughness");
            (*textures).insert((*textures).end(), roughness.begin(), roughness.end());
            std::vector<Texture> ao = loadMaterialTextures(landscape, aiTextureType_AMBIENT_OCCLUSION, "texture_ao");
            (*textures).insert((*textures).end(), ao.begin(), ao.end());
            std::vector<Texture> displacement = loadMaterialTextures(landscape, aiTextureType_DISPLACEMENT, "texture_displacement");
            (*textures).insert((*textures).end(), displacement.begin(), displacement.end());
        }

        m_Meshes.emplace_back(new Mesh(vertices, indices, textures));
        HZ_CORE_INFO("mesh loaded.");
        HZ_CORE_INFO("meshs loaded.");
        HZ_CORE_INFO("model loaded.");
    }
}