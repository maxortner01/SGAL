/**
 * @file   Sizable.h
 * @author Max Ortner
 * @date   4-12-2020
 * 
 * This file contains the class that contains information for sizable objects.
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

#include <SGAL/decl.h>

#include "Math.h"

namespace sgal
{
	class SGAL_API Sizable
	{
	protected:
		Vec2u size;

	public:
		Sizable(Vec2u dimensions);

		Vec2u  getSize()     const;
		double aspectRatio() const;
	};
}