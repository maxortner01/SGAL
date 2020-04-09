#include <SGAL/SGAL.h>

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
        _open = true;
    }

    bool Window::isOpen() const
    {
        return _open;
    }

    void Window::close()
    {
        _open = false;
    }

    void Window::Update()
    {
        UpdateWindow((HWND)settings.handle);
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