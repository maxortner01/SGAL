#include <SGAL/SGAL.h>

#include <iostream>

namespace sgal
{

    LightArray::LightArray() :
        count(0)
    {
        lights = (Light*)std::malloc(sizeof(Light) * SG_MAX_LIGHTS);
    }

    LightArray::~LightArray()
    {
        std::free(lights);
        lights = nullptr;
    }

    void LightArray::push(const Light& light)
    {
        std::memcpy((lights + count++), &light, sizeof(Light));
    }

    Light& LightArray::operator[] (const unsigned int index) const
    {
        SG_ASSERT(index < count, "Requested light doesn't exist!");
        return *(lights + index);
    }

    unsigned int LightArray::size() const
    {
        return count;
    }

}