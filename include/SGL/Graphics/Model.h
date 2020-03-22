#pragma once

#include "../Math.h"
#include "Drawable.h"
#include "RawModel.h"

namespace SGL
{
    class Model : public Drawable, public Transform
    {
        RawModel const* rawModel;

    public:
        Model(RawModel const* rm);

        void draw(Shader* shader = nullptr) const override;
    };
}