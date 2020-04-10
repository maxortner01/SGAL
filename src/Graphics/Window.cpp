#include <SGAL/SGAL.h>
#include <GL/glew.h>

static std::stack<sgal::Event> events;
void makeWindow(unsigned int width, unsigned int height, std::string title, void*& handle, void* window);

#if defined(WIN32) || defined(__WIN32)
#   include "WinPlatform/win32Window.cpp"
#endif

namespace sgal
{
    Window::Window(VideoSettings VideoSettings) :
        settings(VideoSettings)
    {
        makeWindow(settings.width, settings.height, settings.title, settings.handle, this);
        
        // Initialize OpenGL with the window handle
#       ifdef WIN32
        PIXELFORMATDESCRIPTOR pfd = 
        {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
            PFD_TYPE_RGBA,
            32,
            0, 0, 0, 0, 0, 0,
            0,
            0, 
            0,
            0, 0, 0, 0,
            24,
            8,
            0,
            PFD_MAIN_PLANE,
            0,
            0, 0, 0
        };

        // Get the hdc for use in the context
        HDC hdc = GetDC((HWND)settings.handle);
        int pixel_format = ChoosePixelFormat(hdc, &pfd);

        // Assert that the pixel format has been registered and set it
        assert(pixel_format);
        assert(SetPixelFormat(hdc, pixel_format, &pfd));

        // Create the context and make it current
        context.create(hdc);

#       else // Linux Platforms
        assert(false);
#       endif

        context.makeCurrent();

        // Initialize glew and flush error stack
        assert(glewInit() == GLEW_OK);
        while (glGetError() != GL_NO_ERROR);

        glClearColor(0, 0, 0, 1);
        clear();

        // Show the window
#       ifdef WIN32
        for (int i = 0; i < MAX_THREADS; i++)
            if (sWINDOWS[i] == this)
                ShowWindow(WINDOWS[i], SW_SHOWNORMAL);
#       else
        assert(false);
#       endif
        
        _open = true;
    }

    Window::~Window()
    {

    }

    bool Window::isOpen() const
    {
        return _open;
    }

    void Window::close()
    {
        _open = false;
    }

    void Window::clear() const
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Window::update()
    {
        assert(events.size() < 200);

        UpdateWindow((HWND)settings.handle);
        context.swapBuffers();
    }

    VideoSettings Window::getVideoSettings() const
    {
        return settings;
    }
    
    void Window::pushEvent(Event event)
    {
        events.push(event);
    }

    bool Window::poll(Event& event)
    {
        if (events.size() == 0) return false;

        event = events.top();
        events.pop();

        return true;
    }
}