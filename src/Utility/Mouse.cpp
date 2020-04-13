#include <SGAL/SGAL.h>

#include <Windows.h>

namespace sgal
{

    MouseState Mouse::getState()
    {
        MouseState state;

#       ifdef WIN32
        state.left    = (bool)GetAsyncKeyState(Mouse::Key_LEFT);
        state.right   = (bool)GetAsyncKeyState(Mouse::Key_RIGHT);
        state.middle  = (bool)GetAsyncKeyState(Mouse::Key_MIDDLE);
#       else
        INVALID_OPERATING_SYSTEM;
#       endif

        return state;
    }

    Vec2i Mouse::getPosition()
    {
        Vec2i pos;

#       ifdef WIN32
        POINT p;
        SG_ASSERT(GetCursorPos(&p), "Error getting the cursor position");

        pos.x = p.x;
        pos.y = p.y;
#       else
        INVALID_OPERATING_SYSTEM;
#       endif

        return pos;
    }

    Vec2i Mouse::getPosition(const Window& window)
    {
        Vec2i pos;

#       ifdef WIN32
        POINT p;
        SG_ASSERT(ScreenToClient(window.getHandle(), &p), "Error getting the cursor position relative to window!");

        pos.x = p.x;
        pos.y = p.y;
#       else
        INVALID_OPERATING_SYSTEM;
#       endif

        return pos;
    }

}