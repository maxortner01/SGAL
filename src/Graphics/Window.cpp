#include <SGAL/SGAL.h>

#if defined(WIN32) || defined(__WIN32)
#   include <Windows.h>
#endif

#include <iostream>
#include <cassert>

#define MAX_THREADS 3

// Windows API way of collecting the DLL instance and holding its
static HINSTANCE _MODULE;
static HANDLE THREADS   [MAX_THREADS];
static DWORD  THREAD_IDS[MAX_THREADS];

// Forward declaration of the event protocol
LRESULT CALLBACK WindowProc(HWND handle, UINT message, WPARAM wparam, LPARAM lparam);

// This is run when the dll is attached/detatched and collects
// the instance and stores it for use in the window creation
bool APIENTRY DllMain(HMODULE hModule, DWORD rfc, LPVOID reserved)
{
    if (rfc == DLL_PROCESS_ATTACH)
    {
        _MODULE = hModule;
        ZeroMemory(THREADS, sizeof(HANDLE) * MAX_THREADS);

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

// This thread runs for each window, it creates the window and
// passes messages along the pipeline
DWORD WINAPI WindowMessageThread( LPVOID lParam )
{
    sgal::VideoSettings* settings = (sgal::VideoSettings*)lParam;

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

        for (int i = 0; i < MAX_THREADS; i++)
            if (THREADS[i] == NULL)
            {
                THREADS[i] = CreateThread(NULL, 0, WindowMessageThread, &settings, 0, &THREAD_IDS[i]);
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
}