#include <SGAL/SGAL.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>
#include <GL/glew.h>

namespace sgal
{
    static void setParameters(const Texture::Parameters& parameter)
    {
        GLenum type;

        // Set the filter parameters
        switch (parameter.filter)
        {
        case Texture::FilterType::Nearest: type = GL_NEAREST; break;
        case Texture::FilterType::Smooth:  type = GL_SMOOTH;  break;
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, type);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, type);

        // Set the wrap parameters
        switch (parameter.wrap)
        {
        case Texture::WrapType::MirroredRepeat: type = GL_MIRRORED_REPEAT;  break;
        case Texture::WrapType::ClampToEdge:    type = GL_CLAMP_TO_EDGE;    break;
        case Texture::WrapType::Repeat:         type = GL_REPEAT;           break;
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, type);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, type);

    }

    Texture::Texture(unsigned int _layer) :
        size(0, 0)
    {   }

    Texture::Texture(Vec2u dimensions, Texture::Type type, const Parameters& param, unsigned int _layer) :
        Texture(_layer)
    {
        create(dimensions, type, param);
    }
    
	Texture::Texture(Vec2u dimensions, const Parameters& param, unsigned int _layer) :
        Texture(dimensions, Type::Color, param, _layer)
    {   }

    Texture::~Texture()
    {
        if (id) { glDeleteTextures(1, &id); id = 0; }
    }

    void Texture::create(Vec2u dimensions, const Parameters& param)
    {
        create(dimensions, Type::Color, param);
    }

    void Texture::create(Vec2u dimensions, Texture::Type type, const Parameters& param)
    {
        SG_ASSERT(!id, "Texture already created!");
        size       = dimensions;
        parameters = param;

        // Create the texture with OpenGL
        glGenTextures(1, &id);
        bind();
        setParameters(param);

        switch (type)
        {
        case Type::Color:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
            break;

        case Type::Depth:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, size.x, size.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
            break;

        unbind();
        };
    }

    void Texture::fromFile(const std::string& filename)
    {
        stbi_set_flip_vertically_on_load(true);

        int x, y, comp;
        void* data = stbi_load(filename.c_str(), &x, &y, &comp, STBI_rgb_alpha);
        SG_ASSERT(data, "Failure reading image data!");

        create(Vec2u(x, y));

        bind();
        setParameters(parameters);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        unbind();

        stbi_image_free(data);
    }

    void Texture::fromMemory(const void* const data, const Vec2u& size)
    {
        bool created = false;
        if (!id) { create(size); created = true; }

        bind();
        setParameters(parameters);
        if (created) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        else         glTexSubImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        unbind();
    }

    Texture::Parameters Texture::getParameters() const
    {
        return parameters;
    }

    Vec2u Texture::getSize() const
    {
        return size;
    }

    void Texture::bindLayer(const unsigned int layer) const
    {
        glActiveTexture(GL_TEXTURE0 + layer);
        glBindTexture(GL_TEXTURE_2D, id);
    }

    void Texture::unbindLayer(const unsigned int layer) const
    {
        glActiveTexture(GL_TEXTURE0 + layer);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture::bind() const
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, id);
    }

    void Texture::unbind() const
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}