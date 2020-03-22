#pragma once

#include "Shader.h"
#include "Drawable.h"
#include "GL/GLData.h"
#include "../Math.h"

namespace SGL
{
    class Surface : protected GL::Object
    {
        Vec2u _dimensions;

    public:
        Surface(Vec2u dimenions);

        Vec2u getSize() const;
        double aspectRatio() const;

        void clear(Color color = Color(0, 0, 0, 1)) const;
        void draw(const Drawable& drawable, Shader* shader = nullptr) const;

        virtual void bind() const override   { }
        virtual void unbind() const override { }
    };
}