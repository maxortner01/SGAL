#include <SGL/SGL.h>

#include <iostream>
#include <GL/glew.h>

namespace SGL::GL
{
    VBO::VBO(unsigned int _index) :
        dynamic(false), data_bound(false), index(_index)
    {
        glGenBuffers(1, &id);
    }

    VBO::~VBO()
    {   
        glDeleteBuffers(1, &id);
    }

    void VBO::loadData(const void* data, size_t size, size_t count, size_t members)
    {
        bind();
        glBufferData(GL_ARRAY_BUFFER, size * count, data, (dynamic)?(GL_DYNAMIC_DRAW):(GL_STATIC_DRAW));

        if (data_bound) return;

        if (index >= ModelMatrices)
        {
            unsigned int adj_index = ((index - ModelMatrices) * 4) + ModelMatrices;
            
            glEnableVertexAttribArray(adj_index + 0);
            glEnableVertexAttribArray(adj_index + 1);
            glEnableVertexAttribArray(adj_index + 2);
            glEnableVertexAttribArray(adj_index + 3);

            bind();
            glVertexAttribPointer(adj_index + 0, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4f), (void*)(0));
            glVertexAttribPointer(adj_index + 1, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4f), (void*)(sizeof(float) * 4));
            glVertexAttribPointer(adj_index + 2, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4f), (void*)(sizeof(float) * 8));
            glVertexAttribPointer(adj_index + 3, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4f), (void*)(sizeof(float) * 12));

            glVertexAttribDivisor(adj_index + 0, 1);
            glVertexAttribDivisor(adj_index + 1, 1);
            glVertexAttribDivisor(adj_index + 2, 1);
            glVertexAttribDivisor(adj_index + 3, 1);
        }
        else
        {
            glVertexAttribPointer(index, members, GL_FLOAT, false, 0, nullptr);
            glEnableVertexAttribArray(index);
        }

        data_bound = true;
    }

    void VBO::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, id);
    }

    void VBO::unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

#undef LOAD_DATA