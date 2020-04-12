#include <SGL/SGL.h>

#include <cstring>
#include <GL/glew.h>
#include <vector>

#include <iostream>

namespace SGL::GL
{
    VAO::VAO(RenderMode mode, bool _dynamic) :
        vertex_count(0), index_count(0), pointSize(1.f), indices(nullptr),  renderMode(mode), dynamic(_dynamic)
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

    void VAO::loadNormalMatrices(const Mat4f* data, size_t count) const
    {
        if (!count) return;

        bind();
        vbos[NormalMatrices]->loadData(data, sizeof(Mat4f), count, 4 * 4);
        unbind();
    }

    void VAO::loadModelMatrices(const Mat4f* data, size_t count) const
    {
        if (!count) return;

        bind();
        vbos[ModelMatrices]->loadData(data, sizeof(Mat4f), count, 4 * 4);
        unbind();
    }

    void VAO::loadNormals(const Vec3f* data, size_t count) const
    {
        if (!count) return;

        bind();
        vbos[Normals]->loadData(data, sizeof(Vec3f), count, 3);
        unbind();
    }

    void VAO::loadTexCoords(const Vec2f* data, size_t count) const
    {
        if (!count) return;

        bind();
        vbos[TexCoords]->loadData(data, sizeof(Vec2f), count, 2);
        unbind();
    }

    void VAO::loadColors(const Color* data, size_t count) const
    {
        if (!count) return;

        bind();
        vbos[Colors]->loadData(data, sizeof(Color), count, 4);
        unbind();
    }

    void VAO::loadVertices(const Vec3f* data, size_t count)
    {
        if (!count) return;

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

    void VAO::draw(const Surface* surface, Shader* shader, Camera* camera) const
    {
        glDisable(GL_BLEND);
        bind();
        
        vbos[Vertices]->bind();

        glPointSize(pointSize);

        unsigned int type;
        switch (renderMode)
        {
        case Triangles: type = GL_TRIANGLES;    break;
        case Points:    type = GL_POINTS;       break;
        case Lines:     type = GL_LINES;        break;
        };

        if (shader)
        {
            shader->bind();
            if (camera)
            {
                shader->setUniform("view_matrix", camera->getTransformMatrix());
                shader->setUniform("proj_matrix", camera->getProjectionMatrix());
            }
        }

        // Draw the array to the context
        glDrawElements(type, index_count, GL_UNSIGNED_INT, indices);
        glEnable(GL_BLEND);
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