#include <SGAL/SGAL.h>

#include <GL/glew.h>

#ifdef WIN32
#   include <Windows.h>
#endif

namespace sgal
{

    Context::Context() :
        handle(nullptr), hdc(nullptr)
    {

    }

    Context::Context(IPTR _hdc) :
        handle(nullptr), hdc(_hdc)
    {
        create(_hdc);
    }

    Context::~Context()
    {
        if (handle)
        {
            wglMakeCurrent((HDC)hdc, NULL);
            wglDeleteContext((HGLRC)handle);
        }
    }

    void Context::create(IPTR _hdc)
    {
        HGLRC context = wglCreateContext((HDC)_hdc);

        handle = context;
        hdc    = _hdc;
    }

    void Context::makeCurrent() const
    {
        wglMakeCurrent((HDC)hdc, (HGLRC)handle);
    }

    void Context::swapBuffers() const
    {
        SwapBuffers((HDC)hdc);
    }

}