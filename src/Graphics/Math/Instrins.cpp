#include <SGAL/SGAL.h>

#ifdef SGAL_INTRINS

#include <xmmintrin.h>

namespace sgal
{
namespace intrinsics
{
    Vec3f addVectors(const Vec3f& a, const Vec3f& b)
    {
        Vec3f final_result;
        addVectors(a, b, final_result);

        return final_result;
    }

	void addVectors(const Vec3f& a, const Vec3f& b, Vec3f& res)
    {
        __m128 vec_a = { a.x, a.y, a.z };
        __m128 vec_b = { b.x, b.y, b.z };

        __m128 result = _mm_add_ps(vec_a, vec_b);
        float* float_result = (float*)&result;

        res.x = *float_result;
        res.y = *(float_result + 1);
        res.z = *(float_result + 2);
    }

	void addFourFloats(const float* a, const float* b, float* res)
    {
        __m128 vec_a = { a[0], a[1], a[2], a[3] };
        __m128 vec_b = { b[0], b[1], b[2], b[3] };

        __m128 result = _mm_add_ps(vec_a, vec_b);
        
        std::memcpy((void*)res, (const void*)&result, sizeof(float) * 4);
    }

	void multFourFloats(const float* a, const float* b, float* res)
    {
        __m128 vec_a = { a[0], a[1], a[2], a[3] };
        __m128 vec_b = { b[0], b[1], b[2], b[3] };

        __m128 result = _mm_mul_ps(vec_a, vec_b);
        
        std::memcpy((void*)res, (const void*)&result, sizeof(float) * 4);
    }
}
}

#endif