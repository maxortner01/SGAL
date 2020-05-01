#include <SGAL/SGAL.h>
#include <GL/glew.h>

static bool OPENGL_INITIALIZED = false;

void makeWindow(unsigned int width, unsigned int height, std::string title, void*& handle, void* window);

#if defined(WIN32) || defined(__WIN32)
#   include "WinPlatform/win32Window.cpp"
#endif

namespace sgal
{
    Window::Window(VideoSettings videoSettings) :
        Sizable({ videoSettings.width, videoSettings.height }), settings(videoSettings)
    {
        makeWindow(settings.width, settings.height, settings.title, (void*&)settings.handle, this);
        
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
        HDC hdc = GetDC(settings.handle);
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

    void Window::update()
    {
        if (events.size() > 100) while (events.size()) events.pop();

        //SG_ASSERT(events.size() < 200, "Event maximum reached, are you polling?");

#       ifdef WIN32
        UpdateWindow(settings.handle);
#       endif

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

        SG_ASSERT(false, "Something strange happened: Window tried to hide but it's not registered.");
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

        // In the case of a resize, we need to change the
        // size of the Sizable associated with the window
        if (event.type == Event::Resize)
        {
            size.x = event.size.width;
            size.y = event.size.height;
            glViewport(0, 0, size.x, size.y);
        }

        return true;
    }

    void Window::setTitle(const std::string& text) const
    {
#   ifdef WIN32
        std::wstring wTitle(text.length(), L' ');
        std::copy(text.begin(), text.end(), wTitle.begin());

        SG_ASSERT(SetWindowTextW(getHandle(), wTitle.c_str()), "Error setting window text!");
#   else
        INVALID_OPERATING_SYSTEM;
#   endif
    }

    HWND_PTR Window::getHandle() const
    {
        return settings.handle;
    }

    void Window::setPosition(Vec2i pos) 
    {
        position = pos;
    }

    Vec2i Window::getPosition() const
    {
        return position;
    }
}