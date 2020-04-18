/**
 * @file   Rectangle.h
 * @author Max Ortner
 * @date   4-17-2020
 * 
 * Description...
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

#include <SGAL/decl.h>

#include "../Math.h"
#include "../Model.h"

#include "Element.h"

namespace sgal
{
namespace UI
{
	class SGAL_API Rectangle : public UI::Element, public Transform, public Sizable
	{
		RawModel* rawModel;
		Model    model;

		Color color;

	public:
		Rectangle();
		~Rectangle();

		void draw(const SizableSurface* surface) const override;

		void  setColor(const Color& color);
		Color getColor() const;
	};
}
}