#include <SGAL/SGAL.h>

#include <GL/glew.h>

namespace sgal
{
namespace GL
{

    BufferObject::BufferObject(const unsigned int _index) :
        index(_index), dynamic(false), data_bound(false)
    {
        glGenBuffers(1, &id);
    }

    BufferObject::~BufferObject()
    {
        glDeleteBuffers(1, &id);
    }

    void BufferObject::loadData(const IPTR data, const size_t size, const size_t count, const size_t members) 
    {
        bind();

        glBufferData(GL_ARRAY_BUFFER, size * count, data, (dynamic)?GL_DYNAMIC_DRAW:GL_STATIC_DRAW);

        if (data_bound) return;

        if (index >= ModelMatrices)
        {
            unsigned int adj_index = ((index - ModelMatrices) * 4) + ModelMatrices;
            
            glEnableVertexAttribArray(adj_index + 0);
            glEnableVertexAttribArray(adj_index + 1);
            glEnableVertexAttribArray(adj_index + 2);
            glEnableVertexAttribArray(adj_index + 3);

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

    void BufferObject::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, id);
    }

    void BufferObject::unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

}
}