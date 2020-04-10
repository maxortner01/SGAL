#include <Windows.h>
#include <iostream>
#include <cstring>
#include <cassert>
#include <mutex>
#include <thread>
#include <stack>

#define MAX_THREADS 3

namespace sgal
{
    class Event;
    class Window;
}

// Structure for storing the information needed to create a window
struct ThreadInfo
{
    unsigned int width, height, index;
    std::string  title;
};

// Windows API way of collecting the DLL instance and holding its
static HINSTANCE _MODULE;
static std::thread* THREADS [MAX_THREADS];
//static DWORD  THREAD_IDS[MAX_THREADS];
static HWND   WINDOWS   [MAX_THREADS];
static sgal::Window* sWINDOWS[MAX_THREADS];

static std::mutex mtx;

// Forward declaration of the event protocol
LRESULT CALLBACK WindowProc(HWND handle, UINT message, WPARAM wparam, LPARAM lparam);

// This is run when the dll is attached/detatched and collects
// the instance and stores it for use in the window creation
bool APIENTRY DllMain(HMODULE hModule, DWORD rfc, LPVOID reserved)
{
    if (rfc == DLL_PROCESS_ATTACH)
    {
        _MODULE = hModule;
        ZeroMemory(THREADS,    sizeof(std::thread*) * MAX_THREADS);
        //ZeroMemory(THREAD_IDS, sizeof(DWORD)  * MAX_THREADS);
        ZeroMemory(WINDOWS,    sizeof(HWND)   * MAX_THREADS);
        ZeroMemory(sWINDOWS,   sizeof(void*)  * MAX_THREADS);

        WNDCLASSW wc;
        ZeroMemory(&wc, sizeof(WNDCLASS));

        wc.lpszClassName = L"SGAL Window Class";
        wc.hInstance     = _MODULE;
        wc.lpfnWndProc   = WindowProc;
        wc.hCursor       = LoadCursor(NULL, IDC_ARROW);

        assert(RegisterClassW(&wc));
    }
    else if (rfc == DLL_PROCESS_DETACH)
    {
        //PostQuitMessage(0);
        for (int i = 0; i < MAX_THREADS; i++)
        {
            if (THREADS[i]) THREADS[i]->join();
            delete THREADS[i];
        }
    }

    return true;
}

// This thread runs for each window, it creates the window and
// passes messages along the pipeline
void WindowMessageThread( ThreadInfo* lParam )
{
    ThreadInfo info;
    std::memcpy(&info, lParam, sizeof(ThreadInfo));

    // Copy string to wstring
    std::wstring wTitle(info.title.length(), L' ');
    std::copy(info.title.begin(), info.title.end(), wTitle.begin());

    // Create the window and get the handle
    HWND window = CreateWindowW( 
        L"SGAL Window Class",
        wTitle.c_str(),
        WS_OVERLAPPEDWINDOW,

        CW_USEDEFAULT, CW_USEDEFAULT,
        info.width, info.height,

        NULL, NULL,
        _MODULE,
        NULL
    );

    assert(window);

    WINDOWS[info.index] = window;

    MSG message = {};
    while (GetMessage(&message, window, 0, 0))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

extern std::stack<sgal::Event> events;

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
    case WM_CLOSE:
        {
            sgal::Event event;
            event.type = sgal::Event::Closed;

            events.push(event);
        }
        return 0;   

    default:
        return DefWindowProcW(handle, message, wparam, lparam);
    }
}

// Create the window by creating a new messaging thread and passing the handle back to the caller
void makeWindow(unsigned int width, unsigned int height, std::string title, void*& handle, void* window)
{
    assert(!THREADS[MAX_THREADS - 1]);

    ThreadInfo thread_info;
    ZeroMemory(&thread_info, sizeof(ThreadInfo));

    for (int i = 0; i < MAX_THREADS; i++)
        if (THREADS[i] == NULL)
        {
            // Populate the thread_info object
            thread_info.index  = i;
            thread_info.title  = title;
            thread_info.width  = width;
            thread_info.height = height;

            sWINDOWS[i] = (sgal::Window*)window;
            THREADS[i] = new std::thread(WindowMessageThread, &thread_info);

            // Make sure the thread was successfully created
            assert(THREADS[i]);

            // Wait for the thread to open the window,
            // if it never opens an assertion will have failed
            // and this will break.
            while (!WINDOWS[i]);
            handle = WINDOWS[i];

            break;
        }
}