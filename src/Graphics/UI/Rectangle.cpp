#include <SGAL/SGAL.h>

namespace sgal
{
namespace UI
{
    RawModel* Rectangle::rawModel = nullptr;

    static void generateRawModel(RawModel** rm)
    {
        *rm = new RawModel();
        RawModel* rawModel = *rm;

        std::vector<unsigned int> indices(6);
        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;

        indices[3] = 2;
        indices[4] = 3;
        indices[5] = 0;
        rawModel->loadIndices(&indices[0], indices.size());

        std::vector<Vec3f> points(4);
        points[0] = Vec3f(0,  0, 0);
        points[1] = Vec3f(1,  0, 0);
        points[2] = Vec3f(1, -1, 0);
        points[3] = Vec3f(0, -1, 0);
        rawModel->loadVertices(&points[0], points.size());
    }

    Rectangle::Rectangle() :
        size({ 0.f, 0.f }), color(255, 255, 255, 255), radius(0)
    {
        if (!rawModel) generateRawModel(&rawModel);

        model = new Model(rawModel);
    }

    Rectangle::~Rectangle()
    {
        if (model)
        {
            delete model;
            model = nullptr;
        }
    }

    void Rectangle::draw(const SizableSurface* surface) const
    {
        RenderContext rc;
        rc.depth_testing   = false;
        rc.contxt_override = false;

        Shader::DefaultUI().setUniform("size", size);
        Shader::DefaultUI().setUniform("radius", radius);

        // Need a better way
        Color render_color = getColor();
        if (getParent()) render_color.a *= static_cast<Rectangle*>(getParent())->getColor().a;

        rawModel->setColor(render_color);

        surface->draw(*model, &rc);
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

    void Rectangle::setSize(const Vec2f& size)
    {
        this->size = size;
    }

    Vec2f Rectangle::getSize() const
    {
        return size;
    }

    void Rectangle::setRadius(float r)
    {
        radius = r;
    }

    float Rectangle::getRadius() const
    {
        return radius;
    }

    Vec2f Rectangle::getRootPosition() const
    {
        return Vec2f(getPosition().x + radius, getPosition().y + radius);
    }

    Vec3f Rectangle::getRelativePosition() const
    {
        return Vec3f(radius, radius, 0.f);
    }

}
}