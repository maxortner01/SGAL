/**
 * @file   LightArray.h
 * @author Max Ortner
 * @date   4-14-2020
 * 
 * Description...
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

#include <SGAL/decl.h>

#define SG_MAX_LIGHTS 16

namespace sgal
{
	struct Light
	{
		enum Type
		{
			Directional,
			Point
		};

		Vec3f position  = Vec3f(0, 0, 0);
		Type  type      = Type::Directional;
		Color color     = Color(255, 255, 255, 255);
		float intensity = 1.f;
	};	

	class SGAL_API LightArray
	{
		Light* lights;
		unsigned int count;

	public:
		LightArray();
		~LightArray();

		void push(const Light& light);

		Light& operator[] (const unsigned int index) const;

		unsigned int size() const;
	};
}