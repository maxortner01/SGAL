/**
 * @file   ModelArray.h
 * @author Max Ortner
 * @date   4-12-2020
 * 
 * Description...
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

#include <SGAL/decl.h>

#include <vector>

#include "Model.h"

namespace sgal
{
	class SGAL_API ModelArray : public Drawable
	{
		RawModel const* rawModel;

		std::vector<Model> models;

		bool static_render;

	public:
		ModelArray(RawModel const* rm, bool _static = false);

		Model& makeModel();

		void loadMatrices() const;
		void setStaticRender(bool _static);

		void   remove(const unsigned int index);
		Model& get(const unsigned int index);

		unsigned int size() const;

		void draw(const Surface* surface, const RenderContext* rc) const override;
	};
}