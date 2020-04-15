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

#include "../Utility.h"

#include "Math/Vec.h"
#include "Drawable.h"
#include "Sizable.h"

#define SURFACE_BINDS protected:\
	void bindSurface() const override;\
	void unbindSurface() const override;

namespace sgal
{
	struct SGAL_API Surface
	{
		void clear(Color color = Color(0, 0, 0, 255)) const;
		void draw(const Drawable& object, const RenderContext* context = nullptr) const;

	protected:
		virtual void   bindSurface() const = 0;
		virtual void unbindSurface() const = 0;
	};
}