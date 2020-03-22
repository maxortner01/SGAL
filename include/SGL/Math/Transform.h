#pragma once

#include "Vec.h"

namespace SGL
{
    class Transform
    {
        Vec3f position, rotation, scale;

    public:

        Transform() : scale(1, 1, 1)
        {   }

        void setPosition(float x, float y, float z) 
        { setPosition({x, y, z}); }

        void setRotation(float x, float y, float z) 
        { setRotation({x, y, z}); }

        void setScale(float x, float y, float z) 
        { setScale({x, y, z}); }
        
        void addPosition(float x, float y, float z)
        { addPosition({x, y, z}); }
        
        void addRotation(float x, float y, float z)
        { addRotation({x, y, z}); }
        
        void addScale(float x, float y, float z)
        { addScale({x, y, z}); }
    
        void addPosition(Vec3f pos)
        {
            position += pos;
        }
        
        void addRotation(Vec3f rot)
        {
            rotation += rot;
        }
        
        void addScale(Vec3f sc)
        {
            scale += sc;
        }

        void setPosition(Vec3f pos) 
        {
            position = pos;
        }
        
        void setRotation(Vec3f rot) 
        {
            rotation = rot;
        }

        void setScale(Vec3f sc) 
        {
            scale = sc;
        }

        Vec3f getRotation() const { return rotation; }
        Vec3f getPosition() const { return position; }
        Vec3f getScale()    const { return scale;    }
    };
}