#pragma once

#include "../Math.h"

namespace SGL
{
    class Surface;

    class Camera : public Transform
    {
        const Surface * const _surface;
        float FOV;
        Vec2f min_max;

    public:
        Camera(float fov, const Surface& surface);

        Mat4f getProjectionMatrix() const;
        Mat4f getOribitTransform() const;

        void setZFar (float zFar)  { min_max.y = zFar;  }
        void setZNear(float zNear) { min_max.x = zNear; }

        float getZFar()  const { return min_max.y; }
        float getZNear() const { return min_max.x; }
    };
}