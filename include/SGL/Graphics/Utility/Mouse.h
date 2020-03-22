#pragma once

#include "../../Math.h"
#include "../Window.h"

namespace SGL
{
    struct MouseButtons
    {
        bool left = false;
        bool right = false;
        bool middle = false;
        double scroll;
    };

    struct Mouse
    {
        static Vec2f getPosition(const Window& window);
        static MouseButtons getInput(const Window& window);
    };
}