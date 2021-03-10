#pragma once

#include "Geometry.h"
#include "model.h"

namespace Hazel
{

    class SimpleGeometryModel : public Model
    {
    public:
        class Interface
        {
            friend class Actor;
        private:
            static Model* create(void* actor, SimpleGeometryType type) { return new SimpleGeometryModel(actor, type); }
        };

    protected:
        SimpleGeometryModel(void*, SimpleGeometryType);
    };

}


