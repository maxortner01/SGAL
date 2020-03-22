#pragma once

#include "../Math.h"

namespace SGL
{
    class Camera : public Transform
    {
        float FOV;

    public:
        Camera(float fov);
    };
}