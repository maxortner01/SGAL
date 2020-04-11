/**
 * @file   Surface.h
 * @author Max Ortner
 * @date   4-10-2020
 * 
 * A surface object is something upon which a drawable object can be drawn.
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

#include <SGAL/decl.h>

#include "Math/Vec.h"
#include "Drawable.h"

namespace sgal
{
	class SGAL_API Surface
	{
		Vec2u size;

	public:
		Surface(Vec2u dimensions);

		Vec2u  getSize()     const;
		double aspectRatio() const;

		void draw(const Drawable& object) const;
	};
}