#include <SGL/SGL.h>

namespace SGL
{
    Camera::Camera(float fov, const Surface& surface) : 
        FOV(fov), min_max(0.001f, 2000.f), _surface(&surface)
    {
        
    }

    Mat4f Camera::getProjectionMatrix() const
    {
        const float f = 1.f / tanf(FOV / 2.f);

        Mat4f ret;

        ret(0, 0) = 1.0 / _surface->aspectRatio() * f;
        ret(1, 1) = f;
        ret(2, 2) = getZFar() / (getZFar() - getZNear());
        ret(2, 3) = (-getZFar() * getZNear()) / (getZFar() - getZNear());
        ret(3, 2) = 1.f;
        ret(3, 3) = 1.f;

        return ret;
    }

    Mat4f Camera::getOribitTransform() const
    {
        return makeTranslationMatrix(getPosition()) * makeRotationMatrix(getRotation());
    }
}