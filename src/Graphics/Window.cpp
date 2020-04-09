#include <SGAL/SGAL.h>

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
    }

    bool Window::isOpen() const
    {
        return IsWindow((HWND)settings.handle);
    }

    void Window::Update() const
    {
        UpdateWindow((HWND)settings.handle);
    }

    VideoSettings Window::getVideoSettings() const
    {
        return settings;
    }
    
    void Window::pushEvent()
    {

    }
}