/**
 * @file   Drawable.h
 * @author Max Ortner
 * @date   4-10-2020
 * 
 * This file contains the class that represents a drawable object.
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

namespace sgal
{
	class Surface;

	struct Drawable
	{
		virtual void draw(const Surface* surface) const = 0;
	};
}