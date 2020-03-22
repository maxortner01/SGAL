#pragma once

#include <vector>
#include "Drawable.h"
#include "Model.h"

namespace SGL
{
    class ModelArray : public Drawable
    {   
        RawModel const* rawModel;
        std::vector<Model*> models;

        bool static_render;

    public:
        ModelArray(RawModel const* rm, bool _static_render = false);
        ~ModelArray();

        void loadModelMatrices() const;

        Model& makeModel();
        void   remove(unsigned int index);
        Model& get(unsigned int index) const;

        void draw(Shader* shader = nullptr) const override;
    };
}