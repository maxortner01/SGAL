#include <SGAL/SGAL.h>

#include <iostream>
#include <math.h>

namespace sgal
{

    Camera::Camera(const float fov) :
        FOV(fov), orbit(false)
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

    Mat4f Camera::getProjectionMatrix(float aspectRatio) const
    {
        const float f = 1.f / tanf(FOV / 2.f);

        Mat4f proj;

        proj(0, 0) = 1.0 / aspectRatio * f;
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

    void Camera::setFOV(float _fov)
    {
        FOV = _fov;
    }

    float Camera::getZNear() const
    {
        return near_far.x;
    }

    float Camera::getZFar() const
    {
        return near_far.y;
    }

    float Camera::getFOV() const
    {
        return FOV;
    }

    void Camera::setOribitTransform(bool _orbit)
    {
        orbit = _orbit;
    }

    void FPSCamera::update(const Window& window, float speed, float sensitivity)
    {
        Vec2i mouse_delta = Mouse::getPosition(window) - Vec2i(window.getSize().x / 2, window.getSize().y / 2);
        addRotation({ (float)mouse_delta.y / sensitivity, (float)mouse_delta.x / sensitivity, 0 });

        Mouse::setPosition({ (int)(window.getSize().x / 2), (int)(window.getSize().y / 2) }, window);
        
        Vec3f delta;

        if (Keyboard::isKeyPressed(Keyboard::Key_LEFT))
            addRotation({ 0,  0.008f, 0});
        if (Keyboard::isKeyPressed(Keyboard::Key_RIGHT))
            addRotation({ 0, -0.008f, 0});
        if (Keyboard::isKeyPressed(Keyboard::Key_UP))
            addRotation({  0.008f, 0, 0});
        if (Keyboard::isKeyPressed(Keyboard::Key_DOWN))
            addRotation({ -0.008f, 0, 0});

        if (Keyboard::isKeyPressed(Keyboard::Key_LSHIFT))
            speed *= 2.f;
        if (Keyboard::isKeyPressed(Keyboard::Key_A))
            delta.x -= speed;
        if (Keyboard::isKeyPressed(Keyboard::Key_D))
            delta.x += speed;
        if (Keyboard::isKeyPressed(Keyboard::Key_W))
            delta.z += speed;
        if (Keyboard::isKeyPressed(Keyboard::Key_S))
            delta.z -= speed;
        if (Keyboard::isKeyPressed(Keyboard::Key_SPACE))
            delta.y += speed;
        if (Keyboard::isKeyPressed(Keyboard::Key_LCTRL))
            delta.y -= speed;

        step(delta);
    }

}