/**
 * @file   Event.h
 * @author Max Ortner
 * @date   4-9-2020
 * 
 * This is where the events are handled and passed around. This event architecture is heavily
 * inspired by SFML's event handling system.
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

#include "../Utility/Keyboard.h"
#include "../Utility/Mouse.h"

namespace sgal
{
    class Event
    {
        struct ResizeEvent
        {
            unsigned int width, height;
        };

        struct PositionEvent
        {
            int x, y;
        };

        struct KeyEvent
        {
            KeyState          state;
            Keyboard::KeyCode code;
        };

        struct MouseEvent
        {
            Mouse::KeyCode code;
            KeyState       state;
            int x, y;   
        };

        struct WheelEvent
        {
            int delta;
        };

    public:
        enum EventType
        {
            Closed,
            Resize,
            Moving,
            KeyDown,
            KeyHeld,
            KeyUp,
            MouseDown,
            MouseUp,
            WheelMove
        };

        EventType type;

        union
        {
            ResizeEvent   size;
            PositionEvent position;
            KeyEvent      key;
            MouseEvent    mouse;
            WheelEvent    wheel;
        };
        
    };
}