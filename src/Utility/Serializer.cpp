#include <SGAL/SGAL.h>

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
            };

            std::free(data);
        }
    }

}