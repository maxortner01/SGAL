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
	class SGAL_API Rectangle : public UI::Element
	{
		static RawModel* rawModel;
		/****/ Model*    model;

		Texture* texture;

		Color color;
		Vec2f size;
		float radius;

	public:
		Rectangle();
		~Rectangle();

		void draw(const SizableSurface* surface) const override;

		void  setColor(const Color& color);
		Color getColor() const;

		void  setSize(const Vec2f& size);
		Vec2f getSize() const;

		void  setRadius(float r);
		float getRadius() const;

		Vec2f getRootPosition() const;

		Vec3f getRelativePosition() const override;
	};
}
}