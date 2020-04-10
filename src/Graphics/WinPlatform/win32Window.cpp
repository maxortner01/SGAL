#include <Windows.h>
#include <iostream>
#include <cstring>
#include <mutex>
#include <thread>
#include <stack>

#define MAX_THREADS 3

#define INIT_EVENT(event_type) sgal::Event event; event.type = event_type
#define PUSH_EVENT events.push(event);

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
static std::thread* THREADS  [MAX_THREADS];
static HWND   WINDOWS        [MAX_THREADS];
static sgal::Window* sWINDOWS[MAX_THREADS];

extern std::stack<sgal::Event> events;
extern bool                    OPENGL_INITIALIZED;

// State information for the key
enum WIN32_KEY_STATES
{
    KEY_HELD = (1 << 30),
    KEY_UP   = (1 << 31)
};

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

        SG_ASSERT(RegisterClassW(&wc), "Win32 class failed to register!");
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

    SG_ASSERT(window, "Window creation failure!");

    WINDOWS[info.index] = window;

    MSG message = {};
    while (GetMessage(&message, window, 0, 0))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
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
    //
    // Event for closing the window
    //
    case WM_CLOSE:
        {
            sgal::Event event;
            event.type = sgal::Event::Closed;
            events.push(event);
        }
        return 0;   

    //
    // Event for changing the size of the window
    //
    case WM_SIZING:
        {
            RECT* rect = (RECT*)lparam;

            INIT_EVENT(sgal::Event::Resize);
            event.size.width  = rect->right  - rect->left;
            event.size.height = rect->bottom - rect->top;
            PUSH_EVENT;

            // Just to be safe, keep track of OpenGL initialization
            // Change the viewport to the new size of the window
            if (OPENGL_INITIALIZED)
                glViewport(0, 0, event.size.width, event.size.height);
        }
        return 1;

    //
    // Event when the user moves the window
    //
    case WM_MOVING:
        {
            RECT* rect = (RECT*)lparam;

            INIT_EVENT(sgal::Event::Moving);
            event.position.x = rect->left;
            event.position.y = rect->top;
            PUSH_EVENT;
        }
        return 1;

    //
    // Key down event
    //
    case WM_KEYUP:
    case WM_KEYDOWN:
        {
            uint32_t key_flag;
            std::memcpy(&key_flag, &lparam, 32);

            INIT_EVENT(sgal::Event::KeyDown);

            // Check if the key is held down
            bool key_held = (key_flag & KEY_HELD) == KEY_HELD;
            bool key_up   = (key_flag & KEY_UP  ) == KEY_UP; 

            if (key_held)
            {
                event.type      = sgal::Event::KeyHeld;
                event.key.state = sgal::KeyState::KeyHeld;
            }
            else
                event.key.state = sgal::KeyState::KeyDown;

            if (key_up)
            {
                event.type      = sgal::Event::KeyUp;
                event.key.state = sgal::KeyState::KeyUp;
            }
                
            event.key.code = (sgal::Keyboard::KeyCode)wparam;

            PUSH_EVENT;
        }
        return 1;
        
    case WM_LBUTTONDOWN:
        {
            INIT_EVENT(sgal::Event::MouseDown);

            event.mouse.code  = sgal::Mouse::Key_LEFT;
            event.mouse.state = sgal::KeyState::KeyDown;

            event.mouse.x = LOWORD(lparam);
            event.mouse.y = HIWORD(lparam);

            PUSH_EVENT;
        }
        return 1;

    case WM_RBUTTONDOWN:
        {
            INIT_EVENT(sgal::Event::MouseDown);

            event.mouse.code  = sgal::Mouse::Key_RIGHT;
            event.mouse.state = sgal::KeyState::KeyDown;

            event.mouse.x = LOWORD(lparam);
            event.mouse.y = HIWORD(lparam);

            PUSH_EVENT;
        }
        return 1;
        
    case WM_MBUTTONDOWN:
        {
            INIT_EVENT(sgal::Event::MouseDown);

            event.mouse.code  = sgal::Mouse::Key_MIDDLE;
            event.mouse.state = sgal::KeyState::KeyDown;

            event.mouse.x = LOWORD(lparam);
            event.mouse.y = HIWORD(lparam);

            PUSH_EVENT;
        }
        return 1;
    
    case WM_LBUTTONUP:
        {
            INIT_EVENT(sgal::Event::MouseUp);

            event.mouse.code  = sgal::Mouse::Key_LEFT;
            event.mouse.state = sgal::KeyState::KeyUp;

            event.mouse.x = LOWORD(lparam);
            event.mouse.y = HIWORD(lparam);

            PUSH_EVENT;
        }
        return 1;
        
    case WM_RBUTTONUP:
        {
            INIT_EVENT(sgal::Event::MouseUp);

            event.mouse.code  = sgal::Mouse::Key_RIGHT;
            event.mouse.state = sgal::KeyState::KeyUp;

            event.mouse.x = LOWORD(lparam);
            event.mouse.y = HIWORD(lparam);

            PUSH_EVENT;
        }
        return 1;
        
    case WM_MBUTTONUP:
        {
            INIT_EVENT(sgal::Event::MouseUp);

            event.mouse.code  = sgal::Mouse::Key_MIDDLE;
            event.mouse.state = sgal::KeyState::KeyUp;

            event.mouse.x = LOWORD(lparam);
            event.mouse.y = HIWORD(lparam);

            PUSH_EVENT;
        }
        return 1;

    default:
        return DefWindowProcW(handle, message, wparam, lparam);
    }
}

// Create the window by creating a new messaging thread and passing the handle back to the caller
void makeWindow(unsigned int width, unsigned int height, std::string title, void*& handle, void* window)
{
    ThreadInfo thread_info;
    ZeroMemory(&thread_info, sizeof(ThreadInfo));

    for (int i = 0; i < MAX_THREADS; i++)
    {
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
            SG_ASSERT(THREADS[i], "Window thread failed to initialize!");

            // Wait for the thread to open the window,
            // if it never opens an assertion will have failed
            // and this will break.
            while (!WINDOWS[i]);
            handle = WINDOWS[i];

            // Capture keyboard/mouse input on creation
            SetForegroundWindow(WINDOWS[i]);

            break;
        }
        
        // If we get here and its the last one, no thread is open.
        SG_ASSERT(i != MAX_THREADS - 1, "Maximum window count reached!");
    }
}

#undef INIT_EVENT
#undef PUSH_EVENT