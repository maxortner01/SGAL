/**
 * @file   Transform.h
 * @author Max Ortner
 * @date   4-12-2020
 * 
 * Description...
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

#include <SGAL/decl.h>

#include "Matrix.h"

namespace sgal
{
	class SGAL_API Transform
	{
		Vec3f position, rotation, scale;

		Transform* parent;

	public:
		Transform();

		Transform* getParent() const;
		void setParent(Transform* prnt);

		virtual Vec3f getRelativePosition() const;
		virtual Vec3f getRelativeRotation() const;
		virtual Vec3f getRelativeScale() const;

		Vec3f getPosition() const;
		Vec3f getRotation() const;
		Vec3f getScale()    const;

		void addPosition(Vec3f delta);
		void setPosition(float x, float y, float z);
		void setPosition(Vec3f pos);

		void addRotation(Vec3f delta);
		void setRotation(float x, float y, float z);
		void setRotation(Vec3f rot);

		void addScale(Vec3f delta);
		void setScale(float x, float y, float z);
		void setScale(Vec3f sc);

		Mat4f getTransformMatrix() const;
	};
}