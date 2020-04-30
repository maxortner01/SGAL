#include <SGAL/SGAL.h>

#include <iostream>
namespace sgal
{

    bool Keyboard::isKeyPressed(Keyboard::KeyCode key)
    {
#   ifdef WIN32
        return (GetKeyState(key) & 0x8000);
#   else
        INVALID_OPERATING_SYSTEM;
#   endif
    }

}