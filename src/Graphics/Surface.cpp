#include <SGL/SGL.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace SGL
{
    Surface::Surface(Vec2u dimensions) :
        _dimensions(dimensions)
    {   }

    Vec2u Surface::getSize() const
    {
        return _dimensions;
    }

    double Surface::aspectRatio() const
    {
        return (double)_dimensions.x / (double)_dimensions.y;
    }

    void Surface::clear(Color color) const
    {
        bind();
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        unbind();
    }

    void Surface::draw(const Drawable& drawable, Shader* shader, Camera* camera) const
    {
        glEnable(GL_DEPTH_TEST);
        // Bind the correct shader
        if (!shader)    Shader::setDefault();
        else            shader->bind();

        bind();
        drawable.draw(this, shader, camera);
        unbind();
    }
}