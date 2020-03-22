#include <SGL/SGL.h>

#include <iostream>
#include <GL/glew.h>

#define LOAD_DATA(type) glBufferData(type, size * count, data, (dynamic)?(GL_DYNAMIC_DRAW):(GL_STATIC_DRAW));\
        glVertexAttribPointer(index, members, GL_FLOAT, false, 0, nullptr);\
        glEnableVertexAttribArray(index);

namespace SGL::GL
{
    VBO::VBO(unsigned int _index) :
        dynamic(false), index(_index)
    {
        glGenBuffers(1, &id);
    }

    VBO::~VBO()
    {   
        glDeleteBuffers(1, &id);
    }

    void VBO::loadData(const void* data, size_t size, size_t count, size_t members) const
    {
        bind();
        LOAD_DATA(GL_ARRAY_BUFFER);
    }

    void VBO::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, id);
    }

    void VBO::unbind() const
    {
        glBindBuffer(0, GL_ARRAY_BUFFER);
        glBindBuffer(0, GL_ELEMENT_ARRAY_BUFFER);
    }

    
    void IndexBuffer::loadData(const void* data, size_t size, size_t count, size_t members) const
    {
        bind();
        LOAD_DATA(GL_ELEMENT_ARRAY_BUFFER);
    }

    void IndexBuffer::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    }
}

#undef LOAD_DATA