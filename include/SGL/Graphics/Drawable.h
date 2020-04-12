#pragma once

#include "Camera.h"
#include "Shader.h"

namespace SGL
{
    class Surface;

    struct Drawable
    {
        virtual void draw(const Surface* surface = nullptr, Shader* shader = nullptr, Camera* camera = nullptr) const = 0;
    };
}