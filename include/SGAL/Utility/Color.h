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
		float r, g, b, a;

		Color(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a = 255) :
			r((float)_r / 255.f), g((float)_g / 255.f), b((float)_b / 255.f), a((float)_a / 255.f) 
		{	}

		explicit Color(float _r = 255, float _g = 255, float _b = 255, float _a = 255) :
			r(_r), g(_g), b(_b), a(_a) 
		{	}
		
		explicit Color(int _r, int _g, int _b, int _a = 255) :
			Color((unsigned char)_r, (unsigned char)_g, (unsigned char)_b, (unsigned char)_a)
		{	}
	};
}