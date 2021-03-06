#include <SGAL/SGAL.h>

#include <iostream>

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

        std::vector<Vec2f> texCoords(4);
        texCoords[0] = Vec2f(0, 1);
        texCoords[1] = Vec2f(1, 1);
        texCoords[2] = Vec2f(1, 0);
        texCoords[3] = Vec2f(0, 0);
        rawModel->loadTexCoords(&texCoords[0], texCoords.size());
    }

    Rectangle::Rectangle() :
        size({ 0.f, 0.f }), color(255, 255, 255, 255), radius(0), texture(nullptr)
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

    void Rectangle::draw(const SizableSurface* surface, const Shader* const shader) const
    {
        RenderContext rc;
        rc.depth_testing   = false;
        rc.contxt_override = false;

        const Shader* const use_shader = (shader)?(shader):(&Shader::DefaultUI());

        // Set some default UI uniforms
        use_shader->bind();
        use_shader->setUniform("size", size);
        use_shader->setUniform("radius", radius);

        Color render_color = getColor();

        // If the parent is a base/derived from Rectangle, grab its color
        const Rectangle* parent_rect = dynamic_cast<const Rectangle*>(getParent());
        if (parent_rect) render_color.a = parent_rect->getColor().a;

        // Set the color in the VAO
        use_shader->setUniform("render_color", render_color);

        // If there's a texture, bind it and set the uniforms
        use_shader->bind();
        if (texture)
        {
            texture->bind();
            use_shader->setUniform("use_textures", true);
            use_shader->setUniform("texture1", 0);
        }
        else
            use_shader->setUniform("use_textures", false);

        // Pass the model over to the surface
        surface->draw(*model, &rc);

        // To prevent weird things from happening, unbind the texture
        if (texture) texture->unbind();
    }

    void Rectangle::setColor(const Color& color)
    {
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

    void Rectangle::setTexture(const Texture& tex) 
    {
        texture = &tex;
    }

    const Texture& Rectangle::getTexture() const
    {
        return *texture;
    }

    Vec2f Rectangle::getRootPosition() const
    {
        return Vec2f(getPosition().x + radius, getPosition().y + radius);
    }

    Vec3f Rectangle::getRelativePosition() const
    {
        // Should make this an option in case the user wants to ignore the radius
        Vec3f ret = Vec3f(radius, radius, 0.f); 

        if (getParent()) ret = ret + getParent()->getPosition();

        return ret;
    }

}
}