#include <SGAL/SGAL.h>

#ifndef WIN32
#   define WIN32
#endif

#if defined(WIN32) || defined(__WIN32)
#   include <Windows.h>
#endif

#include <iostream>
#include <cassert>

bool APIENTRY DllMain(HMODULE hModule, DWORD rfc, LPVOID reserved)
{
    std::cout << "yo2 " << rfc << "\n";
    return true;
}

LRESULT CALLBACK WindowProc(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
{
    return NULL;
}

namespace sgal
{
    Window::Window(VideoSettings VideoSettings)
    {
#   ifdef WIN32
        std::cout << "WHATS UP\n";
#   endif
    }
}