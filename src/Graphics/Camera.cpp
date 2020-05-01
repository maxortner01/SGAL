#include <SGAL/SGAL.h>

#include <iostream>
#include <math.h>

namespace sgal
{

    Camera::Camera(const float fov) :
        FOV(fov)
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
        return makeRotationMatrix(getRotation()) *
            makeTranslationMatrix(getPosition() * -1);
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

    void FPSCamera::update(const Window& window, float speed, float sensitivity)
    {
        Vec2i mouse_delta = Mouse::getPosition(window) - Vec2i(window.getSize().x / 2, window.getSize().y / 2);
        addRotation({ (float)mouse_delta.y / sensitivity, (float)mouse_delta.x / sensitivity, 0 });

        Mouse::setPosition({ (int)(window.getSize().x / 2), (int)(window.getSize().y / 2) }, window);
        
        Vec3f delta;

        if (Keyboard::isKeyPressed(Keyboard::Left))
            addRotation({ 0,  0.008f, 0});
        if (Keyboard::isKeyPressed(Keyboard::Right))
            addRotation({ 0, -0.008f, 0});
        if (Keyboard::isKeyPressed(Keyboard::Up))
            addRotation({  0.008f, 0, 0});
        if (Keyboard::isKeyPressed(Keyboard::Down))
            addRotation({ -0.008f, 0, 0});

        if (Keyboard::isKeyPressed(Keyboard::LSHIFT))
            speed *= 2.f;
        if (Keyboard::isKeyPressed(Keyboard::A))
            delta.x -= speed;
        if (Keyboard::isKeyPressed(Keyboard::D))
            delta.x += speed;
        if (Keyboard::isKeyPressed(Keyboard::W))
            delta.z += speed;
        if (Keyboard::isKeyPressed(Keyboard::S))
            delta.z -= speed;
        if (Keyboard::isKeyPressed(Keyboard::Space))
            delta.y += speed;
        if (Keyboard::isKeyPressed(Keyboard::LCTRL))
            delta.y -= speed;

        step(delta);
    }

    void OrbitCamera::update(const Window& window, float speed, float sensitivity)
    {
        static Vec2i last_pos;

        Vec2i mouse_delta = Mouse::getPosition(window) - last_pos;
        last_pos = Mouse::getPosition(window);
        //Mouse::setPosition({ (int)(window.getSize().x / 2), (int)(window.getSize().y / 2) }, window);

        MouseState ms = Mouse::getState();
        if (ms.middle)
            addRotation(Vec3f( (float)mouse_delta.y / sensitivity, (float)mouse_delta.x / sensitivity, 0 ));

        setPosition({ 0, 0, (float)ms.wheel * speed });
    }

    Mat4f OrbitCamera::getPerspectiveMatrix() const
    {
        return makeTranslationMatrix(Vec3f(0, 0, getPosition().z * -1)) *
            makeRotationMatrix(getRotation());
    }

}