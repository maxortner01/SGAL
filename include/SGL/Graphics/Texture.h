#pragma once

#include "GL/GLData.h"
#include "Surface.h"
#include "../Math.h"

namespace SGL
{
    enum class TexType
    {
        Color,
        Depth
    };

    class Texture : public GL::Object
    {
        Vec2u dimensions;

    public:
        Texture();
        Texture(Vec2u _dimensions, TexType type = TexType::Color);

        ~Texture();
        
        void create(Vec2u _dimensions, TexType type = TexType::Color);

        void   bind() const override;
        void unbind() const override;
    };
}