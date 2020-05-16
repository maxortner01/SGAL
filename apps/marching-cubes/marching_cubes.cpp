#include <SGAL/SGAL.h>

#include <thread>
#include <cstring>

#define MC_START_GENERATING this->getData()->status |= marching::Generating;
#define MC_END_GENERATING this->getData()->status |= marching::Generated;

namespace marching
{
namespace Triangulation
{
    #include "triangulation.cpp"
}
    enum Status
    {
        Joined     = 1 << 0,
        Generated  = 1 << 1,
        Generating = 1 << 2,
        Idle       = 1 << 3
    };

    template<typename T>
    class ChunkMesh : public sgal::Drawable
    {
    // Variables
    private:
        struct ChunkData
        {
            std:: thread*      thread;
            sgal::VertexArray  array;
            sgal::SingleModel  model;
            int                status;
        };
        ChunkData* data;
        T threshold;

        float grid_size;

        void _join() const;

    protected:
        // When overloading use the end generating macro
        virtual void threadedGen() = 0;

        virtual T getValue(sgal::Vec3f position) const = 0;

        sgal::VertexArray getMarchedCube(unsigned char index, sgal::Vec3f position) const;

    // Methods
    public:

        ChunkData* getData() const;
        ChunkMesh(const T& t, float gridsize = 1.f);
        virtual ~ChunkMesh();

        void generate();
        void draw(const sgal::Surface* surface, const sgal::RenderContext* rc = nullptr) const override;

        void setThreshold(const T& t);
        void setGridSize(float gs);

        T      getThreshold() const;
        Status getStatus() const;
        float  getGridSize() const;
    };

    template<typename T>
    void ChunkMesh<T>::_join() const
    {
		if (!getData()->thread) return;

        if (getStatus() & Generated && !(getStatus() & Joined))
        {
            // Join the thread
            getData()->thread->join();

            // Load the memory into the GPU
            getData()->model.rawModel.fromArray(getData()->array);

            // Clear the RAM
            getData()->array.clear();

            // Delete the thread object
            delete getData()->thread;

            // Clear the variable
            getData()->thread = nullptr;

            // Update the state
            getData()->status |= Joined;
        }
    }

    template<typename T>
    typename ChunkMesh<T>::ChunkData* ChunkMesh<T>::getData() const
    {
        return data;
    }

    template<typename T>
    sgal::VertexArray ChunkMesh<T>::getMarchedCube(unsigned char index, sgal::Vec3f position) const
    {
        using namespace sgal;

        const Vec3f directions[] = {
            Vec3f( 1.f, -1.f,  1.f) * 0.5f,
            Vec3f( 1.f, -1.f, -1.f) * 0.5f,
            Vec3f(-1.f, -1.f, -1.f) * 0.5f,
            Vec3f(-1.f, -1.f,  1.f) * 0.5f,
            Vec3f( 1.f,  1.f,  1.f) * 0.5f,
            Vec3f( 1.f,  1.f, -1.f) * 0.5f,
            Vec3f(-1.f,  1.f, -1.f) * 0.5f,
            Vec3f(-1.f,  1.f,  1.f) * 0.5f
        };

        uint32_t size;
        for (size = 0; Triangulation::triTable[index][size++] != -1;);

        VertexArray va(size);

        for (int i = 0; Triangulation::triTable[index][i] != -1; i++)
        {
            int edgeIndex = Triangulation::triTable[index][i];
            int points[2] = { -1, -1 };
            /**/ if (edgeIndex < 4)
            {
                points[0] = edgeIndex;
                points[1] = (edgeIndex + 1) % 4;
            }
            else if (edgeIndex >= 3 && edgeIndex < 8)
            {
                points[0] = edgeIndex;
                points[1] = ((edgeIndex - 3) % 4) + 4;
            }
            else
            {
                points[0] = edgeIndex - 8;
                points[1] = edgeIndex - 4;
            }

            T values[2] = {
                getValue(directions[points[0]] + position),
                getValue(directions[points[1]] + position)
            };

            float t = (threshold - values[0]) / (values[1] - values[0]);
            Vec3f pos = directions[points[0]] + ((directions[points[0]] - directions[points[1]]) * t);

            Vertex vert;
            vert.position = pos;
            va.push(vert);
        }

        va.calculateNormals();
        return va;
    }

    template<typename T>
    ChunkMesh<T>::ChunkMesh(const T& t, float gridsize) :
        grid_size(gridsize), threshold(t)
    {
        data = new ChunkData;
        data->status = Idle;
        data->thread = nullptr;
    }

    template<typename T>
    ChunkMesh<T>::~ChunkMesh()
    {
        if (data)
        {
            if (data->thread)
            {
                data->thread->join();
                delete data->thread;
                data->thread = nullptr;
            }

            delete data;
            data = nullptr;
        }
    }

    template<typename T>
    void ChunkMesh<T>::generate()
    {
        if (getStatus() & Generating || data->thread) return;
        MC_START_GENERATING;

        data->thread = new std::thread(&ChunkMesh<T>::threadedGen, this);
    }

    template<typename T>
    void ChunkMesh<T>::draw(const sgal::Surface* surface, const sgal::RenderContext* rc) const
    {
        SG_ASSERT(data, "The data has been corrupted!");

        if (!(getStatus() & marching::Joined)) { _join(); return; }

        surface->draw(data->model, rc);
    }

    template<typename T>
    void ChunkMesh<T>::setThreshold(const T& t)
    {
        threshold = t;
    }

    template<typename T>
    void ChunkMesh<T>::setGridSize(float gs)
    {
        grid_size = gs;
    }

    template<typename T>
    T ChunkMesh<T>::getThreshold() const
    {
        return threshold;
    }

    template<typename T>
    Status ChunkMesh<T>::getStatus() const
    {
        SG_ASSERT(data, "The data has been corrupted!");
        return (Status)data->status;
    }

    template<typename T>
    float ChunkMesh<T>::getGridSize() const
    {
        return grid_size;
    }
}

namespace marching
{
    template<typename T>
    class DataMesh : public ChunkMesh<T>
    {
        sgal::Vec3f size;
        T* inData;

    protected:
        void threadedGen() override;

        T getValue(sgal::Vec3f position) const override;

    public:
         DataMesh(const T* _data, sgal::Vec3f dimensions, const T& _threshold);
        ~DataMesh();
    };

    template<typename T>
    void DataMesh<T>::threadedGen()
    {
        using namespace sgal;

        const Vec3f directions[] = {
            Vec3f( 1.f, -1.f,  1.f) * 0.5f,
            Vec3f( 1.f, -1.f, -1.f) * 0.5f,
            Vec3f(-1.f, -1.f, -1.f) * 0.5f,
            Vec3f(-1.f, -1.f,  1.f) * 0.5f,
            Vec3f( 1.f,  1.f,  1.f) * 0.5f,
            Vec3f( 1.f,  1.f, -1.f) * 0.5f,
            Vec3f(-1.f,  1.f, -1.f) * 0.5f,
            Vec3f(-1.f,  1.f,  1.f) * 0.5f
        };

        for (int x = 0; x < size.x - 1; x++)
            for (int y = 0; y < size.y - 1; y++)
                for (int z = 0; z < size.z - 1; z++)
                {
                    const Vec3f position = Vec3f(x, y, z) + Vec3f(0.5f, 0.5f, 0.5f);

                    uint32_t tri_index = 0;
                    for (int i = 0; i < 8; i++)
                    {
                        const Vec3f point_pos = position + directions[i];
                        const float adj_value = getValue(point_pos);

                        if (adj_value < this->getThreshold()) continue;

                        tri_index |= 1 << i;
                    }
                    
                    VertexArray marchedCube = DataMesh<T>::getMarchedCube(tri_index, position);
                    this->getData()->array.append(marchedCube.transform(sgMatT(position)));
                }

        MC_END_GENERATING;
    }

    template<typename T>
    T DataMesh<T>::getValue(sgal::Vec3f postion) const
    {
        const uint32_t index = postion.x + size.x * (postion.y + size.z * postion.z);
        SG_ASSERT(index < size.x * size.y * size.z, "Math error calculating index!");
        return inData[index];
    }

    template<typename T>
    DataMesh<T>::DataMesh(const T* _data, sgal::Vec3f dimensions, const T& _threshold) :
        ChunkMesh<T>(_threshold), size(dimensions)
    {
		inData = (T*)std::malloc((int)(dimensions.x * dimensions.y * dimensions.z) * sizeof(T));
        std::memcpy(inData, _data, (int)(dimensions.x * dimensions.y * dimensions.z) * sizeof(T));
        SG_ASSERT(inData, "Error allocating memory!");
    }

    template<typename T>
    DataMesh<T>::~DataMesh()
    {
        if (inData)
        {
            std::free(inData);
			inData = nullptr;
        }
    }
}