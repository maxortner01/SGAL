#include <SGAL/SGAL.h>

namespace sgal
{

    Surface::Surface(Vec2u dimensions) :
        size(dimensions)
    {

    }

    Vec2u Surface::getSize() const
    {
        return size;
    }

    void Surface::draw(const Drawable& object) const
    {
        object.draw(this);
    }

}