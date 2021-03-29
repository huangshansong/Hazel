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
    QuixelModel::QuixelModel(void* actor, const string& ditectory, QuixelObjectType type, bool gamma)
        : FromResourceModel(actor, ditectory, gamma), m_Type(type)
    {
        loadDirectory();
    }
    void QuixelModel::loadDirectory()
    {
        //set the model's name
        if (m_Type == QuixelObjectType::_3dplant)
        {
            m_Name = m_Directory.substr(m_Directory.find_last_of('V'), m_Directory.find_last_of('/') - m_Directory.find_last_of('V'));
        }
        else
        {
            m_Name = m_Directory.substr(m_Directory.find_last_of('_') + 1, m_Directory.find_last_of('/') - m_Directory.find_last_of('_') - 1);

        }
        //load LODs from 0 to n
        bool stillHaveLODs = true;
        unsigned int LODsNumber = 0;
        while (stillHaveLODs)
        {
            string modelFileName = m_Directory + m_Name + "_LOD" + to_string(LODsNumber) + ".fbx";

            stillHaveLODs = loadModel(modelFileName, LODsNumber);
            if (stillHaveLODs)
            {
                if (m_Type == QuixelObjectType::_3d)
                {
                    QuixelMaterial* material = new QuixelMaterial(m_Directory, QuixelObjectType::_3d, Resolution::_2K, LODsNumber);
                    setModelUniversalMaterial(shared_ptr<Material>(material), LODsNumber);
                }
                LODsNumber++;
            }
            else//failed to read LODn's model file
            {
                break;
            }
        }
        if (m_Type == QuixelObjectType::_3dplant)
        {
            //setModelUniversalTriangleType(MY_OPENGL_TRIANGLE_TYPE::TRIANGLE_STRIP);
        }
    }
}