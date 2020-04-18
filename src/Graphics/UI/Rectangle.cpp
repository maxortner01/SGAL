#include <SGAL/SGAL.h>

namespace sgal
{
namespace UI
{
    // Make it one static VAO with 1 x 1, then pass model matrix in to specify size. 
    // Width  = getSize().x * getScale().x
    // Height = getSize().y * getScale().y
    Rectangle::Rectangle() :
        rawModel(new RawModel()), model(rawModel), Sizable({ 0, 0 })
    {
        rawModel->setDynamic(true);

        std::vector<unsigned int> indices(6);
        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;

        indices[3] = 2;
        indices[4] = 3;
        indices[5] = 0;
        rawModel->loadIndices(&indices[0], indices.size());
    }

    Rectangle::~Rectangle()
    {
        if (rawModel)
        {
            delete rawModel;
            rawModel = nullptr;
        }
    }

    void Rectangle::draw(const SizableSurface* surface) const
    {
        std::vector<Vec3f> points(4);
        points[0] = Vec3f(getPosition().x              , getPosition().y              , 0.f);
        points[1] = Vec3f(getPosition().x + getSize().x, getPosition().y              , 0.f);
        points[2] = Vec3f(getPosition().x + getSize().x, getPosition().y + getSize().y, 0.f);
        points[3] = Vec3f(getPosition().x              , getPosition().y + getSize().y, 0.f);
        rawModel->loadVertices(&points[0], points.size());

        surface->draw(model);
    }

    void Rectangle::setColor(const Color& color)
    {
        std::vector<Color> colors(4);
        for (int i = 0; i < colors.size(); i++)
            colors[i] = color;
        rawModel->loadColors(&colors[0], colors.size());

        this->color = color;
    }

    Color Rectangle::getColor() const
    {
        return color;
    }

}
}