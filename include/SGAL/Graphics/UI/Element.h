/**
 * @file   UIElement.h
 * @author Max Ortner
 * @date   4-17-2020
 * 
 * Description...
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

#include <SGAL/decl.h>

namespace sgal
{
	class  Shader;
	struct SizableSurface;

namespace UI
{
	struct Element : public Transform
	{
		virtual void draw(const SizableSurface* surface, const Shader* const shader = nullptr) const = 0;
	};
}
}