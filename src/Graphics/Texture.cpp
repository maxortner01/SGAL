#include <SGL/SGL.h>

#include <iostream>
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include "SGL/vendor/stb_image.h"

namespace SGL
{
    Texture::Texture() :
        dimensions({ 0, 0 })
    {

    }

    Texture::Texture(Vec2u _dimensions, TexType type) :
        dimensions(_dimensions)
    {   
        create(dimensions, type);
    }

    Texture::~Texture()
    {
        if (id)
        {
            glDeleteTextures(1, &id);
            id = 0;
        }
    }

    void Texture::create(Vec2u _dimensions, TexType type)
    {
        dimensions = _dimensions;
        std::cout << "Creating texture: " << dimensions.x << ", " << dimensions.y << "\n";

        glGenTextures(1, &id);
        bind();

        switch (type)
        {
        case TexType::Color:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dimensions.x, dimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
            break;

        case TexType::Depth:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, dimensions.x, dimensions.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
            break;
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        unbind();
    }

    void Texture::loadFromFile(const std::string& file)
    {
        stbi_set_flip_vertically_on_load(true);
        
        int x, y, comp;
        void* data = stbi_load(file.c_str(), &x, &y, &comp, STBI_rgb_alpha);
        assert(data);

        create(Vec2u( x, y ));
        
        bind();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dimensions.x, dimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        unbind();
    }

    void Texture::bind() const
    {
        glBindTexture(GL_TEXTURE_2D, id);
    }

    void Texture::unbind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}