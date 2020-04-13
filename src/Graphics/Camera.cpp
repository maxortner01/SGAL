#include <SGAL/SGAL.h>

#include <math.h>

namespace sgal
{

    Camera::Camera(float fov, const Sizable& _surface) :
        surface(&_surface), FOV(fov)
    {
        setZNear(0.001f);
        setZFar(1000.f);
    }

    Mat4f Camera::getProjectionMatrix() const
    {
        const float f = 1.f / tanf(FOV / 2.f);

        Mat4f proj;

        proj(0, 0) = 1.0 / surface->aspectRatio() * f;
        proj(1, 1) = f;
        proj(2, 2) = getZFar() / (getZFar() - getZNear());
        proj(2, 3) = (-getZFar() * getZNear()) / (getZFar() - getZNear());
        proj(3, 2) = 1.f;
        proj(3, 3) = 1.f;

        return proj;
    }

    void Camera::setZNear(float _near) 
    {
        near_far.x = _near;
    }
    
    void Camera::setZFar(float _far)
    {
        near_far.y = _far;
    }

    float Camera::getZNear() const
    {
        return near_far.x;
    }

    float Camera::getZFar() const
    {
        return near_far.y;
    }

}