#include <SGAL/SGAL.h>

#include <iostream>
#include <math.h>

namespace sgal
{

    Camera::Camera(const float fov, const Sizable& _surface) :
        surface(&_surface), FOV(fov), orbit(false)
    {
        setZNear(0.001f);
        setZFar(1000.f);
    }

    void Camera::step(Vec3f direction)
    {
        if (direction == Vec3f(0, 0, 0)) return;

        Matrix<float, 1, 4> position;
        position(0, 0) = direction.x;
        position(0, 1) = direction.y;
        position(0, 2) = direction.z;
        position(0, 3) = 1.f;

        Vec3f adj_rotation = Vec3f(0, getRotation().y, 0);
        Matrix<float, 1, 4> rotated = position * makeRotationMatrix(adj_rotation);
        
        Vec3f rotated_vec;
        rotated_vec.x = rotated(0, 0);
        rotated_vec.y = rotated(0, 1);
        rotated_vec.z = rotated(0, 2);

        addPosition({ rotated_vec.x, direction.y, rotated_vec.z });
    }

    Mat4f Camera::getPerspectiveMatrix() const
    {
        if (!orbit)
        {
            return makeRotationMatrix(getRotation()) *
                makeTranslationMatrix(getPosition() * -1);
        }

        return getTransformMatrix();
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

    void Camera::setOribitTransform(bool _orbit)
    {
        orbit = _orbit;
    }

}