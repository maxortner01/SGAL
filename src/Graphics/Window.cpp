#include <SGAL/SGAL.h>

#if defined(WIN32) || defined(__WIN32)
#   include <Windows.h>
#endif

#include <iostream>
#include <cstring>
#include <cassert>

#define MAX_THREADS 3

// Windows API way of collecting the DLL instance and holding its
static HINSTANCE _MODULE;
static HANDLE THREADS   [MAX_THREADS];
static DWORD  THREAD_IDS[MAX_THREADS];
static HWND   WINDOWS   [MAX_THREADS];
static sgal::Window* sWINDOWS[MAX_THREADS];

// Forward declaration of the event protocol
LRESULT CALLBACK WindowProc(HWND handle, UINT message, WPARAM wparam, LPARAM lparam);

// This is run when the dll is attached/detatched and collects
// the instance and stores it for use in the window creation
bool APIENTRY DllMain(HMODULE hModule, DWORD rfc, LPVOID reserved)
{
    if (rfc == DLL_PROCESS_ATTACH)
    {
        _MODULE = hModule;
        ZeroMemory(THREADS,    sizeof(HANDLE) * MAX_THREADS);
        ZeroMemory(THREAD_IDS, sizeof(DWORD)  * MAX_THREADS);
        ZeroMemory(WINDOWS,    sizeof(HWND)   * MAX_THREADS);
        ZeroMemory(sWINDOWS,   sizeof(void*)  * MAX_THREADS);

        WNDCLASSW wc;
        ZeroMemory(&wc, sizeof(WNDCLASS));

        wc.lpszClassName = L"Sample Window Class";
        wc.hInstance     = _MODULE;
        wc.lpfnWndProc   = WindowProc;
        wc.hCursor       = LoadCursor(NULL, IDC_ARROW);

        assert(RegisterClassW(&wc));
    }
    else if (rfc == DLL_PROCESS_DETACH)
    {
        PostQuitMessage(0);
        for (int i = 0; i < MAX_THREADS; i++)
            CloseHandle(THREADS[i]);
    }

    return true;
}

struct ThreadInfo
{
    sgal::VideoSettings* settings;
    unsigned int index;
};

// This thread runs for each window, it creates the window and
// passes messages along the pipeline
DWORD WINAPI WindowMessageThread( LPVOID lParam )
{
    ThreadInfo info;
    std::memcpy(&info, lParam, sizeof(ThreadInfo));

    sgal::VideoSettings* settings = info.settings;

    // Copy string to wstring
    std::wstring wTitle(settings->title.length(), L' ');
    std::copy(settings->title.begin(), settings->title.end(), wTitle.begin());

    // Create the window and get the handle
    HWND window = CreateWindowW( 
        L"Sample Window Class",
        wTitle.c_str(),
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,

        CW_USEDEFAULT, CW_USEDEFAULT,
        settings->width, settings->height,

        NULL, NULL,
        _MODULE,
        NULL
    );

    assert(window);

    WINDOWS[info.index] = window;

    settings->handle = window;
    ShowWindow(window, SW_SHOWNORMAL);

    MSG message = {};
    while (GetMessage(&message, (HWND)settings->handle, 0, 0))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return 0;
}

// This is where all the events go
LRESULT CALLBACK WindowProc(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
{
    sgal::Window* window = nullptr;
    for (int i = 0; i < MAX_THREADS; i++)
        if (WINDOWS[i] == handle)
        {
            window = sWINDOWS[i];
            break;
        }

    // If the window hasn't been registered yet, just forget about it dawg
    if (!window) return DefWindowProcW(handle, message, wparam, lparam);

    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProcW(handle, message, wparam, lparam);
    }
}

namespace sgal
{
    Window::Window(VideoSettings VideoSettings) :
        settings(VideoSettings)
    {
#   ifdef WIN32
        assert(THREADS[MAX_THREADS - 1] == NULL);

        ThreadInfo thread_info;
        for (int i = 0; i < MAX_THREADS; i++)
            if (THREADS[i] == NULL)
            {
                thread_info.index    = i;
                thread_info.settings = &settings;

                sWINDOWS[i] = this;
                THREADS[i] = CreateThread(NULL, 0, WindowMessageThread, &thread_info, 0, &THREAD_IDS[i]);

                assert(THREADS[i]);

                break;
            }

        // Wait for the thread to open the window,
        // if it never opens an assertion will have failed
        // and this will break.
        while (!isOpen());
#   endif
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