/**
 * @file   Instrins.h
 * @author Max Ortner
 * @date   7-25-2020
 * 
 * Description...
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

#include <SGAL/decl.h>

#ifdef SGAL_INTRINS

namespace sgal
{
	template<typename T>
	struct Vec3;

namespace intrinsics
{
	SGAL_API Vec3<float> addVectors(const Vec3<float>& a, const Vec3<float>& b);
	SGAL_API void        addVectors(const Vec3<float>& a, const Vec3<float>& b, Vec3<float>& res);

	SGAL_API void addFourFloats(const float* a, const float* b, float* res);
	SGAL_API void addFourFloats(
		const float* a1, const float* b1, float* res1,
		const float* a2, const float* b2, float* res2,
		const float* a3, const float* b3, float* res3,
		const float* a4, const float* b4, float* res4
		);

	SGAL_API void multFourFloats(const float* a, const float* b, float* res);
	SGAL_API void multFourFloats(
		const float* a1, const float* b1, float* res1,
		const float* a2, const float* b2, float* res2,
		const float* a3, const float* b3, float* res3,
		const float* a4, const float* b4, float* res4
		);
	SGAL_API void multFourFloats(
		const float a1, const float b1, float* res1,
		const float a2, const float b2, float* res2,
		const float a3, const float b3, float* res3,
		const float a4, const float b4, float* res4
		);
}
}

#endif