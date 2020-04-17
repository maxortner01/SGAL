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

#include "Camera.h"
#include "Shader.h"
#include "LightArray.h"

namespace sgal
{
	struct Surface;

	// Storage for scene information (camera, shader, etc).
	struct RenderContext
	{
		bool       turn_to_camera = false;
		bool       use_lighting   = true;
		Shader     const* shader  = nullptr;
		Camera     const* camera  = nullptr;
		LightArray const* lights  = nullptr;
	};

	struct Drawable
	{
		virtual void draw(const Surface* surface, const RenderContext* context = nullptr) const = 0;
	};
}