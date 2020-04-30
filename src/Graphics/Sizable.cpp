#include <SGAL/SGAL.h>

namespace sgal
{

    Sizable::Sizable(Vec2u dimensions) :
        size(dimensions)
    {   }

    double Sizable::aspectRatio() const
    {
        return (double)size.x / (double)size.y;
    }

    Vec2u Sizable::getSize() const
    {
        return size;
    }

}