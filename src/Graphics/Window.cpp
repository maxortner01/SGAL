#include <SGAL/SGAL.h>
#include <GL/glew.h>

static bool                    OPENGL_INITIALIZED = false;
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
        SG_ASSERT(pixel_format, "Invalid Pixel Format!");
        SG_ASSERT(SetPixelFormat(hdc, pixel_format, &pfd), "Pixel format failed to set!");

        // Create the context and make it current
        context.create(hdc);

#       else // Linux Platforms
        INVALID_OPERATING_SYSTEM;
#       endif

        context.makeCurrent();

        if (!OPENGL_INITIALIZED)
        {
            // Initialize glew and flush error stack
            SG_ASSERT(glewInit() == GLEW_OK, "GLEW failed to initialize!");
            while (glGetError() != GL_NO_ERROR);
            OPENGL_INITIALIZED = true;
        }

        glClearColor(0, 0, 0, 1);
        clear();

        // Show the window
        show();
        
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
        SG_ASSERT(events.size() < 200, "Event maximum reached, are you polling?");

        UpdateWindow((HWND)settings.handle);
        context.swapBuffers();
    }

    void Window::show() const
    {
#       ifdef WIN32
        for (int i = 0; i < MAX_THREADS; i++)
            if (sWINDOWS[i] == this)
            {
                ShowWindow(WINDOWS[i], SW_SHOWNORMAL);
                return;
            }

        SG_ASSERT(false, "Something strange happened: Window tried to show but not registered.");
#       else
        INVALID_OPERATING_SYSTEM;
#       endif
    }

    void Window::hide() const
    {
#       ifdef WIN32
        for (int i = 0; i < MAX_THREADS; i++)
            if (sWINDOWS[i] == this)
            {
                ShowWindow(WINDOWS[i], SW_HIDE);
                return;
            }

        SG_ASSERT(false, "Something strange happened: Window tried to hide but not registered.");
#       else
        INVALID_OPERATING_SYSTEM;
#       endif
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