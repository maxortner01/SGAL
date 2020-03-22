#pragma once

#include "Shader.h"
#include "Drawable.h"
#include "Texture.h"

namespace SGL
{
    struct RenderBuffer : GL::Object
    {   
        Texture* texture;

        ~RenderBuffer();

        void create();

        void   bind() const override;
        void unbind() const override;
    };

    class RenderTexture : public Surface, public Drawable
    {
        Vec2u* old_dimensions;

        Texture color;
        RenderBuffer depth;

    public:
        RenderTexture(Vec2u size, bool depth_buffer = false);
        ~RenderTexture();

        const Texture& texture() const;

        void   bind() const override;
        void unbind() const override;

        void draw(Shader* shader = nullptr) const override;
        using Surface::draw;
    };  
}