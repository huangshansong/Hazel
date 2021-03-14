#pragma once 
#include "hzpch.h"

#include "QuixelFile.h"
#include "FromResourceModel.h"

namespace Hazel {

    //currently, there is only one mesh with LODs in a model, so the model and the mesh share the same name, 
    //because that's how it works in Quixel as far as I know
    class HAZEL_API QuixelModel : public FromResourceModel
    {
    public:
        // constructor, added LODs system, so you only need to input a folder path
        QuixelModel(void* actor, const std::string& ditectory, QuixelObjectType type, bool gamma = false);

        QuixelObjectType m_Type;

    protected:
        virtual void loadDirectory() override;

        //just set Universal material for LearnOpenGLModel
        virtual void processMaterial(Mesh* mesh, aiMaterial* material) override {};

        //just set Universal shader for LearnOpenGLModel
        virtual void processShader(Mesh* mesh) override {};
     
    };

}