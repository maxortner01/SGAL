#include <SGL/SGL.h>

#include <GL/glew.h>

#include <iostream>

namespace SGL
{
    RenderBuffer::~RenderBuffer()
    {
        if (texture) 
        {
            delete texture;
            texture = nullptr;
        }

        if (id)
        {
            glDeleteRenderbuffers(1, &id);
            id = 0;
        }
    }

    RenderBuffer::RenderBuffer() :
        texture(nullptr)
    {
        id = 0;
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

    RenderTexture::RenderTexture() :
        Surface({ 0, 0 }), old_dimensions(new Vec2u(0, 0))
    {

    }

    RenderTexture::RenderTexture(Vec2u size, bool depth_buffer) :
        Surface(size), old_dimensions(new Vec2u(0, 0))
    {   
        create(size, depth_buffer);
    }

    RenderTexture::~RenderTexture()
    {
        if (id)
        {
            glDeleteFramebuffers(1, &id);
            id = 0;
        }

        unbind();

        if (old_dimensions)
        {
            delete old_dimensions;
            old_dimensions = nullptr;
        }
    }

    void RenderTexture::create(Vec2u size, bool depth_buffer)
    {
        setSize(size);

        // Create framebuffer
        glGenFramebuffers(1, &id);

        // Bind Framebuffer
        bind();

        // Create the color texture
        color.create({ size.x, size.y });
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color.id, 0);

        if (depth_buffer)
        {
            //depth.texture = new Texture(size, TexType::Depth);

            depth.create();
            depth.bind();
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.x, size.y);
            depth.unbind();
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

        if (old_dimensions && old_dimensions->x != 0)
            glViewport(0, 0, old_dimensions->x, old_dimensions->y);
    }

    void RenderTexture::draw(const Surface* surface, Shader* shader, Camera* camera) const
    {
        Clock clock;

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_DEPTH_TEST);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture().id);

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
        
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
    }
}