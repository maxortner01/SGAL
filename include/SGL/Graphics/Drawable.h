#pragma once

#include "Shader.h"

namespace SGL
{
    struct Drawable
    {
        virtual void draw(Shader* shader = nullptr) const = 0;
    };
}