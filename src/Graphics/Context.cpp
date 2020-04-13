#include <SGAL/SGAL.h>

namespace sgal
{

    Context::Context() :
        handle(nullptr), hdc(nullptr)
    {

    }

    Context::Context(HDC_PTR _hdc) :
        handle(nullptr), hdc(_hdc)
    {
        create(_hdc);
    }

    Context::~Context()
    {
        if (handle)
        {
#           ifdef WIN32
            wglMakeCurrent(hdc, NULL);
            wglDeleteContext(handle);
#           else
            INVALID_OPERATING_SYSTEM;
#           endif

            handle = nullptr;
        }
    }

    void Context::create(HDC_PTR _hdc)
    {
        GLRC_PTR context = nullptr;
        
#       ifdef WIN32
        context = wglCreateContext(_hdc);
#       else
        INVALID_OPERATING_SYSTEM;
#       endif

        handle = context;
        hdc    = _hdc;
    }

    void Context::makeCurrent() const
    {
        wglMakeCurrent(hdc, handle);
    }

    void Context::swapBuffers() const
    {
        SwapBuffers(hdc);
    }

}