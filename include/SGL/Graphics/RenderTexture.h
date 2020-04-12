#pragma once

#include "Shader.h"
#include "Drawable.h"
#include "Texture.h"

namespace SGL
{
    struct RenderBuffer : GL::Object
    {   
        Texture* texture;

        RenderBuffer();
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
        RenderTexture();
        RenderTexture(Vec2u size, bool depth_buffer = false);
        ~RenderTexture();

        void create(Vec2u size, bool depth_buffer = false);

        const Texture& texture() const;

        void   bind() const override;
        void unbind() const override;

        void draw(const Surface* surface = nullptr, Shader* shader = nullptr, Camera* camera = nullptr) const override;
        using Surface::draw;
    };  
}