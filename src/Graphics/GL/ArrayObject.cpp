#include <SGAL/SGAL.h>

#include <vector>
#include <GL/glew.h>

namespace sgal
{
namespace GL
{
    ArrayObject::ArrayObject(RenderMode mode, bool dynamic) :
        _mode(mode), vertex_count(0), index_count(0), indices(nullptr)
    {    
        glCreateVertexArrays(1, &id);

        bind();

        for (int i = 0; i < BufferType::BufferCount; i++)
            vbos[i] = new BufferObject(i);

        setDynamic(dynamic);
    }

    ArrayObject::~ArrayObject()
    {
        for (int i = 0; i < BufferType::BufferCount; i++)
        {
            delete vbos[i];
            vbos[i] = nullptr;
        }

        glDeleteVertexArrays(1, &id);

        if (indices)
        {
            std::free(indices);
            indices = nullptr;
        }
    }

    void ArrayObject::loadNormalMatrices(const Mat4f* matrices, const size_t count) const
    {
        if (!count) return;

        bind();
        vbos[NormalMatrices]->loadData(matrices, sizeof(Mat4f), count, 4 * 4);
        unbind();
    }

    void ArrayObject::loadModelMatrices(const Mat4f* matrices, const size_t count) const
    {
        if (!count) return;

        bind();
        vbos[ModelMatrices]->loadData(matrices, sizeof(Mat4f), count, 4 * 4);
        unbind();
    }

    void ArrayObject::loadIndices(const unsigned int* _indices, const size_t count)
    {
        if (!count) return;

        // If there are already indices in place, free the memory
        if (indices)
        {
            std::free(indices); indices = nullptr;
        }

        // Allocate the memory for the new indices and make sure it was successful
        indices = (unsigned int*)std::malloc(sizeof(unsigned int) * count);
        SG_ASSERT(indices, "Error allocating index buffer.");

        // Copy the memory from the parameter into the new memory
        std::memcpy(indices, _indices, sizeof(unsigned int) * count);
        
        index_count = count;
    }

    void ArrayObject::loadVertices(const Vec3f* vertices, const size_t count)
    {
        if (!count) return;

        std::vector<Color> colors(count);

        for (int i = 0; i < colors.size(); i++)
            colors[i] = Color(255, 255, 255, 255);

        // Bind the buffer and load the data into the GPU
        bind();
        vbos[Vertices]->loadData(vertices, sizeof(Vec3f), count, 3);
        unbind();

        vertex_count = count;

        // If there aren't indices in place generate some
        if (!indices)
        {
            std::vector<unsigned int> index_array(count);
            for (int i = 0; i < count; i++)
                index_array[i] = i;

            loadIndices(&index_array[0], count);
            index_count = count;
        }
    }

    void ArrayObject::loadNormals(const Vec3f* normals, const size_t count) const
    {
        if (!count) return;

        bind();
        vbos[Normals]->loadData(normals, sizeof(Vec3f), count, 3);
        unbind();
    }

    void ArrayObject::loadColors(const Color* colors, const size_t count) const
    {
        if (!count) return;

        bind();
        vbos[Colors]->loadData(colors, sizeof(Color), count, 4);
        unbind();
    }
    
	void ArrayObject::loadTexCoords(const Vec2f* coords, const size_t count) const
    {
        if (!count) return;

        bind();
        vbos[TexCoords]->loadData(coords, sizeof(Vec2f), count, 2);
        unbind();
    }

    void ArrayObject::setRenderMode(RenderMode mode)
    {
        _mode = mode;
    }

    void ArrayObject::setDynamic(bool dynamic) const
    {
        for (int i = 0; i < BufferType::BufferCount; i++)
            vbos[i]->dynamic = dynamic;
    }

    void ArrayObject::bind() const
    {
        glBindVertexArray(id);
    }

    void ArrayObject::unbind() const
    {
        glBindVertexArray(0);
    }

    unsigned int ArrayObject::vertexCount() const
    {
        return vertex_count;
    }

    unsigned int ArrayObject::indexCount() const
    {
        return index_count;
    }

    RenderMode ArrayObject::getRenderMode() const
    {
        return _mode;
    }

    BufferObject& ArrayObject::operator[](const unsigned int index) const
    {
        return *vbos[index];
    }
}
}