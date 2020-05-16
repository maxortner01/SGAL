#include <SGAL/SGAL.h>

#include <GL/glew.h>

namespace sgal
{
namespace GL
{

    BufferObject::BufferObject(const unsigned int _index) :
        index(_index), dynamic(false), data_bound(false), byte_size(0)
    {
        glGenBuffers(1, &id);
    }

    BufferObject::~BufferObject()
    {
        glDeleteBuffers(1, &id);
    }

    unsigned int BufferObject::getByteSize() const
    {
        return byte_size;
    }

    void BufferObject::loadData(const IPTR data, const size_t size, const size_t count, const size_t members, const BufferDataType type) 
    {
        bind();

        byte_size = size * count;
        glBufferData(GL_ARRAY_BUFFER, size * count, data, (dynamic)?GL_DYNAMIC_DRAW:GL_STATIC_DRAW);

        unsigned int gl_type;
        switch (type)
        {
        case Float: gl_type = GL_FLOAT; break;
        case Int:   gl_type = GL_INT;   break;
        };

        if (data_bound) return;

        if (index >= ModelMatrices)
        {
            unsigned int adj_index = ((index - ModelMatrices) * 4) + ModelMatrices;
            
            glEnableVertexAttribArray(adj_index + 0);
            glEnableVertexAttribArray(adj_index + 1);
            glEnableVertexAttribArray(adj_index + 2);
            glEnableVertexAttribArray(adj_index + 3);

            glVertexAttribPointer(adj_index + 0, 4, GL_FLOAT, gl_type, sizeof(Mat4f), (void*)(0));
            glVertexAttribPointer(adj_index + 1, 4, GL_FLOAT, gl_type, sizeof(Mat4f), (void*)(sizeof(float) * 4));
            glVertexAttribPointer(adj_index + 2, 4, GL_FLOAT, gl_type, sizeof(Mat4f), (void*)(sizeof(float) * 8));
            glVertexAttribPointer(adj_index + 3, 4, GL_FLOAT, gl_type, sizeof(Mat4f), (void*)(sizeof(float) * 12));

            glVertexAttribDivisor(adj_index + 0, 1);
            glVertexAttribDivisor(adj_index + 1, 1);
            glVertexAttribDivisor(adj_index + 2, 1);
            glVertexAttribDivisor(adj_index + 3, 1);
        }
        else
        {
            glVertexAttribPointer(index, members, gl_type, false, 0, nullptr);
            glEnableVertexAttribArray(index);
        }

        data_bound = true;
    }

    void* BufferObject::readData() const
    {
        if (!byte_size) return nullptr;

        void* data_ptr = std::malloc(byte_size);

        if (!data_ptr) return nullptr;

        bind();
        glGetBufferSubData(GL_ARRAY_BUFFER, 0, byte_size, data_ptr);
        unbind();

        return data_ptr;
    }

    void BufferObject::bindBase(unsigned int layer) const
    {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, layer, id);
    }

    void BufferObject::unbindBase(unsigned int layer) const
    {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, layer, 0);
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