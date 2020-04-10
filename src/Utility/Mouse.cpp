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

}