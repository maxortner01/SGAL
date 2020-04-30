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
	/**
	 * @brief One individual instance of a given RawModel to be rendered.
	 * 
	 */
	class SGAL_API Model : public Drawable, public Transform
	{
		RawModel const* rawModel;

		RawModel* normalsRawModel;
		Model*    normalsModel;

	public:
		Model(RawModel const* rm);
		~Model();

		void drawNormals(const Surface& surface, RenderContext* rc);

		void draw(const Surface* surface, const RenderContext* rc = nullptr) const override;
	};
}