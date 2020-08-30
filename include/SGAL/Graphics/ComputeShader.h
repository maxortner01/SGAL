/**
 * @file   ComputeShader.h
 * @author Max Ortner
 * @date   5-7-2020
 * 
 * Description...
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

#include <SGAL/decl.h>

#include "Shader.h"

namespace sgal
{
	class SGAL_API ComputeShader : public Shader
	{
	public:
		BASE_CONSTRUCTOR(Shader);

		void dispatch(const Vec3i size) const;
	};
}