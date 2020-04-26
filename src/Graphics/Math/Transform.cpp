#include <SGAL/SGAL.h>

namespace sgal
{

    Transform::Transform() :
        scale(1, 1, 1), parent(nullptr)
    {   }

    Transform* Transform::getParent() const
    {
        return parent;
    }

    void Transform::setParent(Transform* prnt)
    {
        /**/ if (prnt && parent != prnt)
            prnt->addChild(this);
        else if (!prnt && parent)
            parent->removeChild(this);

        parent = prnt;
    }

    void Transform::addChild(Transform* transform)
    {
        children.push_back(transform);
    }

    void Transform::removeChild(Transform* transform)
    {
        for (int i = 0; i < children.size(); i++)
            if (children[i] == transform)
            {
                children.erase(children.begin() + i);
                return;
            }
        
        SG_ASSERT(false, "Child does not exist!");
    }

    Transform::Children Transform::getChildren() const
    {
        return { &children[0], children.size() };
    }

    Vec3f Transform::getRelativePosition() const
    {
        return Vec3f(0, 0, 0);
    }

    Vec3f Transform::getRelativeRotation() const
    {
        return Vec3f(0, 0, 0);
    }

    Vec3f Transform::getRelativeScale() const
    {
        return Vec3f(1, 1, 1);
    }

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
        Vec3f pos = (parent)?(getPosition() + parent->getPosition() + parent->getRelativePosition()):getPosition();
        Vec3f rot = (parent)?(getPosition() + parent->getRotation() + parent->getRelativeRotation()):getRotation();
        Vec3f scl = (parent)?(getScale() * parent->getScale() * parent->getRelativeScale()):getScale();

        return makeTranslationMatrix(pos) *
            makeRotationMatrix(rot) *
            makeScaleMatrix(scl);
    }

}