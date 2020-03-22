#include <SGL/SGL.h>

#include <cstring>
#include <GL/glew.h>
#include <vector>

namespace SGL::GL
{
    VAO::VAO(RenderMode mode, bool _dynamic) :
        vertex_count(0), index_count(0), indices(nullptr),  renderMode(mode), dynamic(_dynamic)
    {
        glCreateVertexArrays(1, &id);
        
        bind();

        // Generate the VBOS
        for (int i = 0; i < BufferCount; i++)
            vbos[i] = new VBO(i);

        setDynamic(_dynamic);
    }

    VAO::~VAO()
    {
        for (int i = 0; i < BufferCount; i++)
            delete vbos[i];

        glDeleteVertexArrays(1, &id);

        if (indices) std::free(indices);
    }

    void VAO::loadModelMatrices(const Mat4f* data, size_t count) const
    {
        bind();

        vbos[ModelMatrices]->bind();
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(Mat4f), data, GL_DYNAMIC_DRAW);
        vbos[ModelMatrices]->unbind();

        unsigned int index = ModelMatrices;
        
        glEnableVertexAttribArray(index + 0);
        glEnableVertexAttribArray(index + 1);
        glEnableVertexAttribArray(index + 2);
        glEnableVertexAttribArray(index + 3);

        vbos[ModelMatrices]->bind();
        glVertexAttribPointer(index + 0, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4f), (void*)(0));
        glVertexAttribPointer(index + 1, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4f), (void*)(sizeof(float) * 4));
        glVertexAttribPointer(index + 2, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4f), (void*)(sizeof(float) * 8));
        glVertexAttribPointer(index + 3, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4f), (void*)(sizeof(float) * 12));

        glVertexAttribDivisor(index + 0, 1);
        glVertexAttribDivisor(index + 1, 1);
        glVertexAttribDivisor(index + 2, 1);
        glVertexAttribDivisor(index + 3, 1);

        unbind();
    }

    void VAO::loadNormals(const Vec3f* data, size_t count) const
    {
        bind();
        vbos[Normals]->loadData(data, sizeof(Vec3f), count, 3);
        unbind();
    }

    void VAO::loadTexCoords(const Vec2f* data, size_t count) const
    {
        bind();
        vbos[TexCoords]->loadData(data, sizeof(Vec2f), count, 2);
        unbind();
    }

    void VAO::loadColors(const Color* data, size_t count) const
    {
        bind();
        vbos[Colors]->loadData(data, sizeof(Color), count, 4);
        unbind();
    }

    void VAO::loadVertices(const Vec3f* data, size_t count)
    {
        bind();
        vbos[Vertices]->loadData(data, sizeof(Vec3f), count, 3);
        unbind();

        vertex_count = count;

        if (!indices)
        {
            index_count = count;

            std::vector<unsigned int> indicies(vertex_count);
            for (int i = 0; i < vertex_count; i++)
                indicies[i] = i;

            loadIndices(&indicies[0], indicies.size());
        }
    }
    
    void VAO::loadIndices(const unsigned int* index, size_t count)
    {
        index_count = count;
        if (indices) std::free(indices);

        indices = (unsigned int*)std::malloc(count * sizeof(unsigned int));
        std::memcpy(indices, index, count * sizeof(unsigned int));
    }

    void VAO::draw(Shader* shader) const
    {
        bind();
        
        vbos[Vertices]->bind();

        unsigned int type;
        switch (renderMode)
        {
        case Triangles: type = GL_TRIANGLES;    break;
        case Points:    type = GL_POINTS;       break;
        case Lines:     type = GL_LINES;        break;
        };

        // Draw the array to the context
        glDrawElements(type, index_count, GL_UNSIGNED_INT, indices);
    }

    void VAO::bind() const
    {
        glBindVertexArray(id);
    }

    void VAO::unbind() const
    {
        glBindVertexArray(0);
    }

    VBO& VAO::operator[] (unsigned int index) const
    {
        return *vbos[index];
    }

    void VAO::setDynamic(bool _dynamic)
    {
        for (int i = 0; i < BufferCount; i++)
            vbos[i]->dynamic = _dynamic;
    }
}