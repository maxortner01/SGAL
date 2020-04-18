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

#include <math.h>

namespace sgal
{
	template<typename T>
	struct Vec2
	{
		T x, y;

		Vec2(T _x = 0, T _y = 0) :
			x(_x), y(_y)
		{	}

		void operator=(const Vec2<T>& vec)
		{
			x = vec.x; y = vec.y;
		}
	
		Vec2<T> operator+(const Vec2<T>& vec) const
		{
			return Vec2<T>(x + vec.x, y + vec.y);
		}

		Vec2<T> operator*(const T scalar) const
		{
			return Vec2<T>(x * scalar, y * scalar);
		}

		Vec2<T> operator-(const Vec2<T>& vec) const
		{
			return vec + (*this) * -1.f;
		}

		void operator+=(const Vec2<T>& vec)
		{
			(*this) = (*this) + vec;
		}

		template<typename F>
		operator Vec2<F>() const
		{
			return Vec2<F>((F)x, (F)y);
		}
	};

	template<typename T>
	struct Vec3
	{
		T x, y, z;

		Vec3(T _x = 0, T _y = 0, T _z = 0) :
			x(_x), y(_y), z(_z)
		{	}

		void operator=(const Vec3<T>& vec)
		{
			x = vec.x; y = vec.y; z = vec.z;
		}

		Vec3<T> operator+(const Vec3<T>& vec) const
		{
			return Vec3<T>(x + vec.x, y + vec.y, z + vec.z);
		}

		Vec3<T> operator*(const T scalar) const
		{
			return Vec3<T>(x * scalar, y * scalar, z * scalar);
		}

		Vec3<T> operator/(const T scalar) const
		{
			return Vec3<T>(x / scalar, y / scalar, z / scalar);
		}

		Vec3<T> operator-(const Vec3<T>& vec) const
		{
			return vec + (*this) * -1.f;
		}

		void operator+=(const Vec3<T>& vec)
		{
			(*this) = (*this) + vec;
		}

		bool operator==(const Vec3<T>& vec) const
		{
			return ((x == vec.x) && (y == vec.y) && (z == vec.z));
		}

		template<typename F>
		operator Vec3<F>() const
		{
			return Vec3<F>((F)x, (F)y, (F)z);
		}
	};

	typedef Vec2<unsigned int> Vec2u;
	typedef Vec2<float>        Vec2f;
	typedef Vec2<int>          Vec2i;
	
	typedef Vec3<unsigned int> Vec3u;
	typedef Vec3<float>        Vec3f;
	typedef Vec3<int>          Vec3i;

	 template<typename T, typename U = double>
    static U dot(const Vec3<T>& vec1, const Vec3<T>& vec2)
    {
        return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
    }

    template<typename T>
    static Vec3<T> normalize(const Vec3<T>& vec)
    {
        return vec / sqrt(dot(vec, vec));
    }

    template<typename T>
    static Vec3<T> cross(const Vec3<T>& vec1, const Vec3<T>& vec2)
    {
        Vec3<T> ret;

        ret.x = vec1.y * vec2.z - vec1.z * vec2.y;
        ret.y = vec1.z * vec2.x - vec1.x * vec2.z;
        ret.z = vec1.x * vec2.y - vec1.y * vec2.x;

        return ret;
    }
}