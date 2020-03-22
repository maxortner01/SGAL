#pragma once

#include <string>

namespace SGL
{
    class RawModel : public GL::VAO
    {
        friend class ModelArray;
        friend class Model;

    public:
        void loadFromFile(const std::string& location);

        void calculateNormals(
            const Vec3f* data, size_t count, const unsigned int* indices = nullptr, size_t index_count = 0) const;
    };
}