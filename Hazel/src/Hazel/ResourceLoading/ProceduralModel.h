#pragma once
#include "hzpch.h"

#include "model.h"

namespace Hazel
{
    class HAZEL_API ProceduralModel : public Model
    {
    public:
        ProceduralModel(void* actor);

    protected:
        virtual void setupModel() = 0;

        virtual void processMesh() = 0;
    };
};


