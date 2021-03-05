#pragma once 
#include "hzpch.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Hazel/Actors/landscape.h"

#include "mesh.h"
#include "shader.h"

using namespace std;

namespace Hazel {

    class Model
    {
    public:
        
        // constructor, expects a filepath to a 3D model.
        Model(string const& path,  bool fromQuixel, bool gamma = false);
        // constructor, expects a heightMap, for landscape
        Model(Landscape* landscape, bool gamma = false);

        // draws the model, and thus all its meshes
        void draw();

        // model data 
        vector<Mesh*> m_Meshes;


    private:

        
        // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
        void loadModel(string const& path);

        // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
        void processNode(aiNode* node, const aiScene* scene);

        Mesh* processMesh(aiMesh* mesh, const aiScene* scene);

        // checks all material textures of a given type and loads the textures if they're not loaded yet.
        // the required info is returned as a Texture struct.
        vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

        vector<Texture> loadMaterialTextures(Landscape* landscape, aiTextureType type, string typeName);

        // loads a model from heightMap and stores the resulting meshes in the meshes vector.
        void loadModel(Landscape* landscape);

        // model data 
        string m_Directory;
        string m_ModelName;
        bool m_GammaCorrection;
        bool m_FromQuixel;
        vector<Texture> m_Textures_Loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.

    };

}