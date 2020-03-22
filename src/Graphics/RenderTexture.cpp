#include <SGL/SGL.h>

#include <GL/glew.h>

#include <iostream>

namespace SGL
{
    RenderBuffer::~RenderBuffer()
    {
        if (texture) delete texture;
        glDeleteRenderbuffers(1, &id);
    }
    
    void RenderBuffer::create()
    {   
        glGenRenderbuffers(1, &id);
    }

    void RenderBuffer::bind() const
    {
        glBindRenderbuffer(GL_RENDERBUFFER, id);
    }

    void RenderBuffer::unbind() const
    {
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    RenderTexture::RenderTexture(Vec2u size, bool depth_buffer) :
        Surface(size), old_dimensions(new Vec2u(0, 0))
    {   
        // Create framebuffer
        glGenFramebuffers(1, &id);

        // Bind Framebuffer
        bind();

        // Create the color texture
        color.create({ size.x, size.y });
        color.bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color.id, 0);
        color.unbind();

        if (depth_buffer)
        {
            depth.texture = new Texture(size, TexType::Depth);

            depth.create();
            depth.bind();
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.x, size.y);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth.id);
        }

        // Make sure everything works
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout << "Framebuffer ERROR!\n";
            std::getchar();
        }

        // Remove framebuffer
        unbind();
    }

    RenderTexture::~RenderTexture()
    {
        delete old_dimensions;
    }

    const Texture& RenderTexture::texture() const
    {
        return color;
    }

    void RenderTexture::bind() const
    {
        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        if (viewport[2] != getSize().x && viewport[3] != getSize().y)
            *old_dimensions = { (unsigned int)viewport[2], (unsigned int)viewport[3] };

        glBindFramebuffer(GL_FRAMEBUFFER, id);

        glViewport(0, 0, getSize().x, getSize().y);
    }

    void RenderTexture::unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        if (old_dimensions->x != 0)
            glViewport(0, 0, old_dimensions->x, old_dimensions->y);
    }

    void RenderTexture::draw(Shader* shader) const
    {
        unbind();

        glDisable(GL_DEPTH_TEST);

        glEnable(GL_TEXTURE_2D);
        texture().bind();

        glBegin(GL_TRIANGLES);
            glTexCoord2f(0, 0);
            glVertex2f(-1, -1);
            glTexCoord2f(0, 1);
            glVertex2f(-1,  1);
            glTexCoord2f(1, 0);
            glVertex2f( 1, -1);

            glTexCoord2f(1, 1);
            glVertex2f( 1,  1);
            glTexCoord2f(0, 1);
            glVertex2f(-1,  1);
            glTexCoord2f(1, 0);
            glVertex2f( 1, -1);
        glEnd();

        texture().unbind();
        
        glEnable(GL_DEPTH_TEST);
    }
}