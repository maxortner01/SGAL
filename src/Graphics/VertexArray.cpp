#include <SGAL/SGAL.h>

#include <iostream>
#include <cstring>

#define INDEX_OUT_RANGE SG_ASSERT(index <= c_size, "VertexArray out of range!")

namespace sgal
{

    VertexArray::VertexArray(const uint32_t _size) :
        vertices(nullptr), iterator(0)
    {
        resize(_size);
    }

    VertexArray::VertexArray(const VertexArray& array) :
        VertexArray()
    {
        clear();
        resize(array.size());
        std::memcpy(vertices, array.vertices, sizeof(Vertex) * array.size());
        iterator = array.size();
    }
    
    VertexArray::~VertexArray()
    {
        if (vertices)
        {
            std::free(vertices);
            vertices = nullptr;
        }
    }

    void VertexArray::resize(const uint32_t _size)
    {
        if (!_size) return;

        void* newptr = std::malloc(sizeof(Vertex) * _size);
        SG_ASSERT(newptr, "Error resizing array!");
        
        std::memset(newptr, 0, sizeof(Vertex) * _size);

        if (vertices)
        {
            std::memcpy(newptr, vertices, sizeof(Vertex) * c_size);
            std::free(vertices);
        }

        vertices = (Vertex*)newptr;
        c_size   = _size;
    }

    void VertexArray::push(Vertex& vertex) 
    {
        if (iterator >= c_size)
            resize(c_size * 2);
        
        std::memcpy(vertices + iterator++, &vertex, sizeof(Vertex));
    }

    void VertexArray::fit()
    {
        resize(iterator + 1);
    }

    void VertexArray::clear()
    {
        if (!vertices) return;
        
        std::memset(vertices, 0, sizeof(Vertex) * c_size);
        std::free(vertices);
        vertices = nullptr;
    }

    void VertexArray::calculateNormals()
    {
        if (!size()) return;

        std::vector<Vec3f> normals(size());

        for (int i = 0; i < size(); i += 3)
        {
            Vec3f v1, v2, v3;

            v1 = (*this)[i + 0].position;
            v2 = (*this)[i + 1].position;
            v3 = (*this)[i + 2].position;
            
            Vec3f rel_v2 = v2 - v1;
            Vec3f rel_v3 = v3 - v1;

            normals[i + 0] += cross(rel_v2, rel_v3);
            normals[i + 1] += cross(rel_v2, rel_v3);
            normals[i + 2] += cross(rel_v2, rel_v3);
        }
        
        for (int i = 0; i < size(); i++)
        {
            normals[i] = normalize(normals[i]);
            (*this)[i].normal = normals[i];
        }
    }

    void VertexArray::append(const VertexArray& array)
    {
        if (!vertices || iterator + array.size() >= c_size)
            resize(iterator + array.size());
        
        std::memcpy(vertices + iterator, &array[0], sizeof(Vertex) * array.size());
        iterator += array.size();
    }
    
    VertexArray& VertexArray::operator=(const VertexArray& va)
    {
        clear();
        append(va);
        return *this;
    }

    VertexArray VertexArray::transform(const Mat4f& transformation) const
    {
        VertexArray r(size());

        for (int i = 0; i < size(); i++)
        {
            Matrix<float, 4, 1> pos, norm, newpos, newnorm;
            pos(0, 0) = get(i).position.x;
            pos(1, 0) = get(i).position.y;
            pos(2, 0) = get(i).position.z;
            pos(3, 0) = 1;

            norm(0, 0) = get(i).normal.x;
            norm(1, 0) = get(i).normal.y;
            norm(2, 0) = get(i).normal.z;
            norm(3, 0) = 1;

            // Remove translation from transformation
            Mat4f normaltransform = transformation;
            normaltransform(0, 3) = 0;
            normaltransform(1, 3) = 0;
            normaltransform(2, 3) = 0;

            // Extract scale
            double scales[3] = {
                length(Vec3f(normaltransform(0, 0), normaltransform(1, 0), normaltransform(2, 0))),
                length(Vec3f(normaltransform(0, 1), normaltransform(1, 1), normaltransform(2, 1))),
                length(Vec3f(normaltransform(0, 2), normaltransform(1, 2), normaltransform(2, 2)))
            };

            for (int r = 0; r < 3; r++)
                for (int c = 0; c < 3; c++)
                    normaltransform(r, c) /= scales[c];

            newpos  = transformation  * pos;
            newnorm = normaltransform * norm;

            Vertex newPos = get(i);
            newPos.position.x = newpos(0, 0);
            newPos.position.y = newpos(0, 1);
            newPos.position.z = newpos(0, 2);

            newPos.normal.x = newnorm(0, 0);
            newPos.normal.y = newnorm(0, 1);
            newPos.normal.z = newnorm(0, 2);
            newPos.normal = normalize(newPos.normal);

            r.push(newPos);
        }

        return r;
    }

    VertexArray VertexArray::index(const unsigned int* indices, const uint32_t size) const
    {
        VertexArray r(size);

        for (int i = 0; i < size; i++)
        {
            SG_ASSERT(indices[i] < this->size(), "Index out of range!");
            r.push(get(indices[i]));
        }

        return r;
    }

    bool VertexArray::typeFilled(GL::BufferType type) const
    {
        switch (type)
        {
        case GL::Vertices:  return (getVertices().size()  > 0);
        case GL::Normals:   return (getNormals().size()   > 0);
        case GL::TexCoords: return (getTexCoords().size() > 0);
        case GL::Colors:    return (getColors().size()    > 0);
        }

        return false;
    }

    Vertex& VertexArray::get(const uint32_t index) const
    {
        INDEX_OUT_RANGE;
        return *(vertices + index);
    }

    Vertex& VertexArray::operator[](const uint32_t index) const
    {
        return get(index);
    }
    
    uint32_t VertexArray::allocated() const
    {
        return c_size;
    }

    uint32_t VertexArray::size() const
    {
        return iterator;
    }
    
    std::vector<Vec2f> VertexArray::getTexCoords() const
    {
        std::vector<Vec2f> tex;

        bool used = false;
        for (int i = 0; i < size(); i++)
        {
            Vec2f coord = get(i).texture_coord;
            tex.push_back(coord);

            if (!used && dot(coord, coord)) used = true;
        }

        if (!used) tex.clear();

        return tex;
    }

    std::vector<Vec3f> VertexArray::getVertices() const
    {
        std::vector<Vec3f> vertices;

        bool used = false;
        for (int i = 0; i < size(); i++)
        {
            Vec3f vertex = get(i).position;
            vertices.push_back(vertex);

            if (!used && dot(vertex, vertex)) used = true;
        }

        if (!used) vertices.clear();

        return vertices;
    }

    std::vector<Vec3f> VertexArray::getNormals() const
    {
        std::vector<Vec3f> normals;

        bool used = false;
        for (int i = 0; i < size(); i++)
        {
            Vec3f normal = get(i).normal;
            normals.push_back(normal);

            if (!used && dot(normal, normal)) used = true;
        }

        if (!used) normals.clear();

        return normals;
    }

    std::vector<Color> VertexArray::getColors() const
    {
        std::vector<Color> colors;

        bool used = false;
        for (int i = 0; i < size(); i++)
        {
            Color color = get(i).color;
            colors.push_back(color);

            if (!used && color.r + color.g + color.b + color.a != 0.f) used = true;
        }

        if (!used) colors.clear();

        return colors;
    }


}

#undef INDEX_OUT_RANGE