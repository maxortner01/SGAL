#include <SGAL/SGAL.h>

#include <GL/glew.h>

namespace sgal
{

#pragma region RenderBuffer

    DrawTexture::RenderBuffer::RenderBuffer()
    {
        id = 0;
    }
    
    DrawTexture::RenderBuffer::~RenderBuffer()
    {
        if (id)
        {
            glDeleteRenderbuffers(1, &id);
            id = 0;
        }
    }

    void DrawTexture::RenderBuffer::create()
    {
        glGenRenderbuffers(1, &id);
    }

    void DrawTexture::RenderBuffer::bind() const
    {
        glBindRenderbuffer(GL_RENDERBUFFER, id);
    }
    
    void DrawTexture::RenderBuffer::unbind() const
    {
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

#pragma endregion
#pragma region DrawTexture

    void DrawTexture::bind() const
    {
        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        if (last_dimensions && viewport[2] != getSize().x && viewport[3] != getSize().y)
            *last_dimensions = { (unsigned int)viewport[2], (unsigned int)viewport[3] };

        glBindFramebuffer(GL_FRAMEBUFFER, id);

        glViewport(0, 0, getSize().x, getSize().y);
    }

    void DrawTexture::unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        if (last_dimensions && last_dimensions->x != 0)
            glViewport(0, 0, last_dimensions->x, last_dimensions->y);
    }

    DrawTexture::DrawTexture() :
        Sizable({ 0, 0 })
    {   }

    DrawTexture::DrawTexture(const Vec2u& size, bool depth_buffer) :
        Sizable(size)
    {
        create(size, depth_buffer);
    }

    DrawTexture::~DrawTexture()
    {
        if (id)
        {
            glDeleteFramebuffers(1, &id);
            id = 0;
        }

        if (last_dimensions)
        {
            delete last_dimensions;
            last_dimensions = nullptr;
        }
    }

    const Texture& DrawTexture::texture() const
    {
        return color;
    }

    void DrawTexture::render()
    {
        unbind();

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

    void DrawTexture::create(const Vec2u& size, bool depth_buffer)
    {
        last_dimensions = new Vec2u();

        this->size = size;

        glGenFramebuffers(1, &id);

        bind();

        color.create(size);
        color.bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color.id, 0);
        color.unbind();

        if (depth_buffer)
        {
            depth.create();
            depth.bind();
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.x, size.y);
            depth.unbind();

            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth.id);
        }

        SG_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "DrawTexture not successfully created!");

        unbind();
    }

    void DrawTexture::bindSurface() const
    {
        bind();
    }

    void DrawTexture::unbindSurface() const
    {
        unbind();
    }

#pragma endregion

}