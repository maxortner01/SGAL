#include <SGAL/SGAL.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <GL/glew.h>

namespace sgal
{

    Texture::Texture() :
        size(0, 0)
    {

    }

    Texture::Texture(Vec2u dimensions, Texture::Type type) :
        Texture()
    {
        create(dimensions, type);
    }

    Texture::~Texture()
    {
        if (id) { glDeleteTextures(1, &id); id = 0; }
    }

    void Texture::create(Vec2u dimensions, Texture::Type type)
    {
        SG_ASSERT(!id, "Texture already created!");
        size = dimensions;

        glGenTextures(1, &id);
        bind();

        switch (type)
        {
        case Type::Color:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
            break;

        case Type::Depth:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, size.x, size.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
            break;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
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