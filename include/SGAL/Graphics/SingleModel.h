/**
 * @file   SingleModel.h
 * @author Max Ortner
 * @date   4-30-2020
 * 
 * Description...
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

#include <SGAL/decl.h>

namespace sgal
{
	struct SGAL_API SingleModel : Drawable
	{
		SingleModel();

		void draw(const Surface* surface, const RenderContext* rc = nullptr) const override;

		RawModel rawModel;
		Model    model;
	};
}