#include <SGAL/SGAL.h>

#include <GL/glew.h>

namespace sgal
{

    void Surface::clear(Color color) const
    {
        glClearColor((float)color.r / 255.f, (float)color.g / 255.f, (float)color.b / 255.f, (float)color.a / 255.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Surface::draw(const UI::Element& uielem) const
    {
        bindSurface();

        SizableSurface* surface = (SizableSurface*)this;

        Shader::DefaultUI().bind();

        Shader::DefaultUI().setUniform("screen_size", (Vec2f)surface->getSize());

        uielem.draw(surface);

        unbindSurface();
    }

    void Surface::draw(const Drawable& object, const RenderContext* context) const
    {
        bindSurface();
        object.draw(this, context);
        unbindSurface();
    }

}