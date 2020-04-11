/**
 * @file   Vec.h
 * @author Max Ortner
 * @date   4-10-2020
 * 
 * This file contains all the vector math things.
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

namespace sgal
{
	template<typename T>
	struct Vec2
	{
		T x, y;

		Vec2(T _x = 0, T _y = 0) :
			x(_x), y(_y)
		{	}
	};

	template<typename T>
	struct Vec3
	{
		T x, y, z;

		Vec3(T _x = 0, T _y = 0, T _z = 0) :
			x(_x), y(_y), z(_z)
		{	}
	};

	typedef Vec2<unsigned int> Vec2u;
	typedef Vec2<float>        Vec2f;
	typedef Vec2<int>          Vec2i;
	
	typedef Vec3<unsigned int> Vec3u;
	typedef Vec3<float>        Vec3f;
	typedef Vec3<int>          Vec3i;
}