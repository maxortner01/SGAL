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

    void Mouse::showCursor(bool show)
    {
#       ifdef WIN32
        ShowCursor(show);
#       else
        INVALID_OPERATING_SYSTEM;
#       endif
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
#       ifdef WIN32
        return window.getPosition() - getPosition();
#       else
        INVALID_OPERATING_SYSTEM;
#       endif
    }

    void Mouse::setPosition(Vec2i position)
    {
#       ifdef WIN32
        SG_ASSERT(SetCursorPos(position.x, position.y), "Error setting the cursor position!");
#       else
        INVALID_OPERATING_SYSTEM;
#       endif
    }

    void Mouse::setPosition(Vec2i position, const Window& window)
    {
#       ifdef WIN32
        setPosition(position + window.getPosition());
#       else
        INVALID_OPERATING_SYSTEM;
#       endif
    }

}