#include <SGAL/SGAL.h>

namespace sgal
{

    Transform::Transform() :
        scale(1, 1, 1)
    {   }

    Vec3f Transform::getPosition() const
    {
        return position;
    }

    Vec3f Transform::getRotation() const
    {
        return rotation;
    }

    Vec3f Transform::getScale() const
    {
        return scale;
    }

    void Transform::addPosition(Vec3f delta)
    {
        position += delta;
    }

    void Transform::setPosition(float x, float y, float z)
    { setPosition({ x, y, z }); }
    void Transform::setPosition(Vec3f pos)
    {
        position = pos;
    }

    void Transform::addRotation(Vec3f delta)
    {
        rotation += delta;
    }

    void Transform::setRotation(float x, float y, float z)
    { setRotation({ x, y, z }); }
    void Transform::setRotation(Vec3f rot)
    {
        rotation = rot;
    }

    void Transform::addScale(Vec3f delta)
    {
        scale += delta;
    }

    void Transform::setScale(float x, float y, float z)
    { setScale({ x, y, z }); }
    void Transform::setScale(Vec3f sc)
    {
        scale = sc;
    }

    Mat4f Transform::getTransformMatrix() const
    {
        return makeTranslationMatrix(getPosition()) *
            makeRotationMatrix(getRotation()) *
            makeScaleMatrix(getScale());
    }

}