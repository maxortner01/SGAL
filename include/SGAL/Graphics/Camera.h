/**
 * @file   Camera.h
 * @author Max Ortner
 * @date   4-12-2020
 * 
 * Description...
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

#include <SGAL/decl.h>

#include "Math.h"

namespace sgal
{
	class Sizable;

	class SGAL_API Camera : public Transform
	{
		const Sizable* const surface;
		float FOV;
		bool  orbit;
		Vec2f near_far;

	public:
		Camera(float fov, const Sizable& _surface);

		Mat4f getPerspectiveMatrix() const;
		Mat4f getProjectionMatrix()  const;

		void setZNear(float _near);
		void setZFar (float _far);

		float getZNear() const;
		float getZFar()  const;

		void setOribitTransform(bool _orbit);
	};
}