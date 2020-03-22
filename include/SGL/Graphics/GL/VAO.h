#pragma once

#include "../Math.h"
#include "../Drawable.h"
#include "VBO.h"

namespace SGL::GL
{
    enum BufferIndex
    {
        Vertices,
        Colors,
        TexCoords,
        Normals,
        ModelMatrices,
        BufferCount
    };

    enum RenderMode
    {
        Points, Lines, Triangles
    };
    
    struct VAO : Object, public Drawable
    {
        VAO(RenderMode mode = Triangles, bool _dynamic = false);
        ~VAO();

        void loadModelMatrices(const Mat4f* matrices, size_t count) const;
        void loadNormals(const Vec3f* normals, size_t count) const;
        void loadTexCoords(const Vec2f* data, size_t count) const;
        void loadColors(const Color* data, size_t count) const;
        void loadVertices(const Vec3f* data, size_t count);
        void loadIndices(const unsigned int* index, size_t count);

        void draw(Shader* shader = nullptr) const override;

        void bind()   const override;
        void unbind() const override;

        VBO& operator[] (unsigned int index) const;
    
        void setMode(RenderMode mode) { renderMode = mode; }

        void setDynamic(bool _dynamic);

        unsigned int getVertexCount() const { return vertex_count; }
        unsigned int getIndexCount()  const { return index_count; }

    protected:
        VBO* vbos[BufferCount];
        unsigned int* indices;
        RenderMode renderMode;

    private:
        unsigned int index_count, vertex_count;

        bool dynamic;
    };
}