/**
 * @file   Color.h
 * @author Max Ortner
 * @date   4-12-2020
 * 
 * Description...
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

#include <SGAL/decl.h>

namespace sgal
{
	struct SGAL_API Color
	{
		uint8_t r, g, b, a;

		Color(uint8_t _r = 255, uint8_t _g = 255, uint8_t _b = 255, uint8_t _a = 255) :
			r(_r), g(_g), b(_b), a(_a) 
		{	}
	};
}