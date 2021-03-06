#include <SGAL/SGAL.h>

#include <sstream>
#include <cstring>
#include <fstream>

namespace sgal
{

    void Serializer::saveToFile(const std::string& filename, const RawModel& rawModel)
    {
        std::ofstream file(filename, std::ios::out | std::ios::binary);
        SG_ASSERT(file, "Error opening file!");

        for (int i = 0; i < GL::BufferCount; i++)
        {
            const unsigned char BUFFER_TYPE = i;
            const unsigned int  BUFFER_SIZE = rawModel[i].getByteSize();

            if (!BUFFER_SIZE) continue;

            const void* const BUFFER = rawModel[i].readData();  
            
            file.write((const char*)&BUFFER_TYPE, sizeof(unsigned char));
            file.write((const char*)&BUFFER_SIZE, sizeof(unsigned int));
            file.write((const char*)BUFFER,       BUFFER_SIZE);
        }

        if (rawModel.indexCount())
        {
            const unsigned char INDEX_BUFFER = 254;
            const unsigned int  BUFFER_SIZE  = rawModel.indexCount() * sizeof(unsigned int);
            file.write((const char*)&INDEX_BUFFER, sizeof(unsigned char));
            file.write((const char*)&BUFFER_SIZE,  sizeof(unsigned int ));
            file.write((const char*)rawModel.getIndices(), BUFFER_SIZE);
        }

        file.close();
    }

    void Serializer::loadFromFile(const std::string& filename, RawModel& rawModel)
    {
        std::ifstream file(filename, std::ios::in | std::ios::binary);
        SG_ASSERT(file, "Error reading from file!");

        unsigned char buffer_index = 255;
        while (file.read((char*)&buffer_index, sizeof(unsigned char)))
        {
            unsigned int buffer_size;
            void* data;
            file.read((char*)&buffer_size, sizeof(unsigned int));

            data = std::malloc(buffer_size);
            file.read((char*)data, buffer_size);

            // Explicitly load each buffer
            switch (buffer_index)
            {
            case GL::Vertices:       rawModel.loadVertices ((const Vec3f*)data, buffer_size / sizeof(Vec3f)); break;
            case GL::Normals:        rawModel.loadNormals  ((const Vec3f*)data, buffer_size / sizeof(Vec3f)); break;
            case GL::Colors:         rawModel.loadColors   ((const Color*)data, buffer_size / sizeof(Color)); break;
            case GL::TexCoords:      rawModel.loadTexCoords((const Vec2f*)data, buffer_size / sizeof(Vec2f)); break;
            case GL::ModelMatrices:  rawModel.loadModelMatrices ((const Mat4f*)data, buffer_size / sizeof(Mat4f)); break;
            case GL::NormalMatrices: rawModel.loadNormalMatrices((const Mat4f*)data, buffer_size / sizeof(Mat4f)); break;
            case 254: // indices
                rawModel.loadIndices((const unsigned int*)data, buffer_size / sizeof(unsigned int));
                break;
            };

            std::free(data);
        }

        file.close();
    }

    void Serializer::loadFromFile(const std::string& filename, VertexArray& array)
    {
        std::ifstream file(filename, std::ios::in | std::ios::binary);
        SG_ASSERT(file, "Error reading from file!");

        // Seek to the end of the file and get size
        file.seekg(0, std::ios_base::end);
        const unsigned int size = file.tellg();

        // Go back to the beginning
        file.seekg(0);
        unsigned char* const data = (unsigned char*)std::malloc(size);
        file.read((char* const)data, size);
        file.close();

        loadFromArray(data, size, array);

        std::free(data);
    }
    
	void Serializer::loadFromArray(const unsigned char* array, const uint32_t size, VertexArray& varray)
    {
        uint32_t iterator = 0;

        std::vector<unsigned int> indices;
        std::vector<Vec3f> v_vertices, v_normals;
        std::vector<Vec2f> v_texcoords;
        std::vector<Color> v_colors;

        unsigned char buffer_index = 255;
        while (iterator < size)
        {
            unsigned int buffer_size;
            void* data;

            // Read the buffer identifier
            std::memcpy(&buffer_index, array + iterator, sizeof(unsigned char));
            iterator += sizeof(unsigned char);

            // Read the buffer size number
            std::memcpy(&buffer_size, array + iterator, sizeof(unsigned int));
            iterator += sizeof(unsigned int);

            data = std::malloc(buffer_size);
            // Read the data
            std::memcpy(data, array + iterator, buffer_size);
            iterator += buffer_size;

            switch (buffer_index)
            {
                case GL::Vertices:
                {
                    v_vertices.resize(buffer_size / sizeof(Vec3f));
                    std::memcpy(&v_vertices[0], data, buffer_size);
                    break;
                }

                case GL::Normals:
                {
                    v_normals.resize(buffer_size / sizeof(Vec3f));
                    std::memcpy(&v_normals[0], data, buffer_size);
                    break;
                }

                case GL::TexCoords:
                {
                    v_texcoords.resize(buffer_size / sizeof(Vec2f));
                    std::memcpy(&v_texcoords[0], data, buffer_size);
                    break;
                }

                case GL::Colors:
                {
                    v_colors.resize(buffer_size / sizeof(Color));
                    std::memcpy(&v_colors[0], data, buffer_size);
                    break;
                }

                case 254: //indices
                {
                    indices.resize(buffer_size / sizeof(unsigned int));
                    std::memcpy(&indices[0], data, buffer_size);
                    break;
                }
            }

            std::free(data);
        }

        if (!indices.size())
            for (int i = 0; i < v_vertices.size(); i++)
                indices.push_back(i);

        for (int i = 0; i < indices.size(); i++)
        {
            Vertex vertex;
            if (v_texcoords.size())
                vertex.texture_coord = v_texcoords[indices[i]];

            if (v_vertices.size())
                vertex.position      = v_vertices [indices[i]];

            if (v_normals.size())
                vertex.normal        = v_normals  [indices[i]];

            if (v_colors.size())
                vertex.color         = v_colors   [indices[i]];

            varray.push(vertex);
        }
    }

}