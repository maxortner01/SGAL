#include <SGAL/SGAL.h>

#include <GL/glew.h>



#include <iostream>
namespace sgal
{

    void Surface::clear(Color color) const
    {
        bindSurface();
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        unbindSurface();
    }

    void Surface::draw(const UI::Element& uielem) const
    {
        bindSurface();

        SizableSurface* surface = (SizableSurface*)this;

        Shader::DefaultUI().bind();

        Shader::DefaultUI().setUniform("screen_size", (Vec2f)surface->getSize());
        Shader::DefaultUI().setUniform("transform_mat", uielem.getTransformMatrix());

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