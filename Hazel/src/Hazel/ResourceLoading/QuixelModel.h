#pragma once 
#include "hzpch.h"

#include "QuixelFile.h"
#include "model.h"

namespace Hazel {

    //currently, there is only one mesh with LODs in a model, so the model and the mesh share the same name, 
    //because that's how it works in Quixel as far as I know
    class QuixelModel : public Model
    //currently, there is only one mesh with LODs in a model, so the model and the mesh share the same name, 
    //because that's how it works in Quixel as far as I know

    {
    public:
        class Interface
        {
            friend class Actor;
        private:
            static Model* create(void* actor, const std::string& folderPath, QuixelObjectType type, bool gamma = false) 
            { return new QuixelModel(actor, folderPath, type, gamma); }
        };

    protected:
        // constructor, added LODs system, so you only need to input a folder path
        QuixelModel(void* actor, const std::string& folderPath, QuixelObjectType type, bool gamma);
 
        std::vector<std::vector<Mesh*>> m_LODs;

        QuixelObjectType m_Type;
    };

}