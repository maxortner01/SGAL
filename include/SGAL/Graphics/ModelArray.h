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

		void loadMatrices() const;

	public:
		ModelArray(RawModel const* rm);

		Model& makeModel();

		void   remove(const unsigned int index);
		Model& get(const unsigned int index);

		void draw(const Surface* surface, const RenderContext* rc) const override;
	};
}