#include "hzpch.h"

#include "Hazel/Core.h"
#include "Hazel/Log.h"

#include "Material.h"
#include "model.h"
#include "QuixelModel.h"

using namespace std;
namespace Hazel
{
    // constructor, expects a filepath to a 3D model.
    QuixelModel::QuixelModel(void* actor, const string& folderPath, QuixelObjectType type, bool gamma)
        : Model(actor, gamma), m_Type(type)
    {
        m_Directory = folderPath;

        //set the model's name, only for the Quxel's model
        m_Name = folderPath.substr(folderPath.find_last_of('/') + 1, folderPath.find_last_of('_') - folderPath.find_last_of('/') - 1);

        //load LODs from 0 to n
        bool stillHaveLODs = true;
        while (stillHaveLODs)
        {
            string modelFileName = folderPath + m_Name + "_LOD" + to_string(m_LODs.size()) + ".fbx";
            
            std::vector<Mesh*> meshes;
            stillHaveLODs = loadModel(modelFileName);
            if (stillHaveLODs)
            {
                meshes.clear();
                meshes = m_Meshes;
                m_LODs.emplace_back(meshes);
                m_Meshes.clear();

                for (Mesh* mesh : meshes)
                {
                    setMeshQuixelMaterial(mesh, folderPath, m_TexturesLoaded, m_Type, m_LODs.size());
                }
            }
            else//failed to read LODn's model file
            {
                break;
            }  
        }
    }
}