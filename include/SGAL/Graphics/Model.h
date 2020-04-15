/**
 * @file   Model.h
 * @author Max Ortner
 * @date   4-12-2020
 * 
 * Description...
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

#include <SGAL/decl.h>

#include "Drawable.h"
#include "RawModel.h"
#include "Math.h"

namespace sgal
{
	class SGAL_API Model : public Drawable, public Transform
	{
		RawModel const* rawModel;

	public:
		Model(RawModel const* rm);

		void draw(const Surface* surface, const RenderContext* rc = nullptr) const override;
	};
}