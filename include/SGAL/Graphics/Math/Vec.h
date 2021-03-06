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

#include "SGAL/decl.h"
#include "Instrins.h"
#include <math.h>
#include <ostream>

namespace sgal
{
	/**
	 * @brief Represents a two-dimensional vector.
	 * 
	 * @tparam T Type of data contained in the vector
	 */
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
	
		template<typename F>
		Vec2<F> operator+(const Vec2<F>& vec) const
		{
			return Vec2<F>(x + vec.x, y + vec.y);
		}

		template<typename F>
		Vec2<F> operator*(const F scalar) const
		{
			return Vec2<F>(x * scalar, y * scalar);
		}
		
		template<typename F>
		Vec2<F> operator/(const F scalar) const
		{
			return Vec2<F>(x / scalar, y / scalar);
		}

		template<typename F>
		Vec2<F> operator-(const Vec2<F>& vec) const
		{
			return Vec2<F>(x - vec.x, y - vec.y);
		}

		void operator+=(const Vec2<T>& vec)
		{
			(*this) = (*this) + vec;
		}

		template<typename F>
		bool operator==(const Vec2<F>& vec) const
		{
			return (x == vec.x && y == vec.y);
		}

		template<typename F>
		operator Vec2<F>() const
		{
			return Vec2<F>((F)x, (F)y);
		}
	};

	/**
	 * @brief Represents a three-dimensional vector.
	 * 
	 * @tparam T Type of data contained in the vector
	 */
	template<typename T>
	struct Vec3
	{
		T x, y, z;

		Vec3(T _x = 0, T _y = 0, T _z = 0) :
			x(_x), y(_y), z(_z)
		{	}

		void operator=(const Vec3<T>& vec)
		{
			std::memcpy(this, &vec, sizeof(T) * 3);
		}

		Vec3<T> operator+(const Vec3<T>& vec) const
		{
#		ifndef SGAL_INTRINS
			return Vec3<T>(x + vec.x, y + vec.y, z + vec.z);
#		else
			return intrinsics::addVectors(*this, vec);
#		endif
		}


		Vec3<T> operator*(const T scalar) const
		{
			return Vec3<T>(x * scalar, y * scalar, z * scalar);
		}
		
		Vec3<T> operator*(const Vec3<T> vec) const
		{
			return Vec3<T>(x * vec.x, y * vec.y, z * vec.z);
		}

		Vec3<T> operator/(const T scalar) const
		{
			return Vec3<T>(x / scalar, y / scalar, z / scalar);
		}

		Vec3<T> operator-(const Vec3<T>& vec) const
		{
			return Vec3<T>(x - vec.x, y - vec.y, z - vec.z);
		}

		void operator+=(const Vec3<T>& vec)
		{
#		ifndef SGAL_INTRINS
			this.x += vec.x;
			this.y += vec.y;
			this.z += vec.z;
#		else
			intrinsics::addVectors(*this, vec, *this);
#		endif
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

		friend std::ostream& operator<<(std::ostream& os, const Vec3<T>& dt)
		{
			os << dt.x << ", " << dt.y << ", " << dt.z;
			return os;
		}
	};

	typedef Vec2<unsigned int> Vec2u;
	typedef Vec2<double>       Vec2d;
	typedef Vec2<float>        Vec2f;
	typedef Vec2<int>          Vec2i;
	
	typedef Vec3<unsigned int> Vec3u;
	typedef Vec3<double>       Vec3d;
	typedef Vec3<float>        Vec3f;
	typedef Vec3<int>          Vec3i;

	/**
	 * @brief Gets the dot-product of two three-dimensional vectors.
	 * 
	 * This operation is communitive so the order of vec1 and vec2 does not matter.
	 * 
	 * @tparam T 	Type of the constituents
	 * @tparam U 	Return type (default: double)
	 * @param vec1 	First vector
	 * @param vec2 	Second vector
	 * @return U 	The dot-product of the vectors
	 */
	template<typename T, typename U = double>
    static U dot(const Vec3<T>& vec1, const Vec3<T>& vec2)
    {
        return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
    }
	
	/**
	 * @brief Gets the dot-product of two two-dimensional vectors.
	 * 
	 * This operation is communitive so the order of vec1 and vec2 does not matter.
	 * 
	 * @tparam T 	Type of the constituents
	 * @tparam U	Return type (default: double)
	 * @param vec1 	First vector
	 * @param vec2 	Second vector
	 * @return U 	The dot-product of the vectors
	 */
	template<typename T, typename U = double>
    static U dot(const Vec2<T>& vec1, const Vec2<T>& vec2)
    {
        return vec1.x * vec2.x + vec1.y * vec2.y;
    }

	template<typename T, typename U = double>
	static U length(const T& vec)
	{
		return sqrt(dot(vec, vec));
	}

	/**
	 * @brief Normalizes the length of the given vector to one.
	 * 
	 * @tparam T 		Type of the constituents
	 * @param vec 		The vector to normalize
	 * @return Vec3<T> 	The normalized vector
	 */
    template<typename T>
    static T normalize(const T& vec)
    {
        return vec / length(vec);
    }

	/**
	 * @brief Gets the cross-product between two vectors.
	 * 
	 * This operation is not communitive and holds the property that
	 * vec2 x vec1 = -(vec1 x vec2).
	 * 
	 * @tparam T 		The type of the constituents
	 * @param vec1 		The first vector
	 * @param vec2 		The second vector
	 * @return Vec3<T> 	The yielded vector from the cross-product
	 */
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

// math overloads
namespace std
{
	template<typename T>
	static sgal::Vec3<T> cos(const sgal::Vec3<T>& vec)
	{
		return sgal::Vec3<T>(cos(vec.x), cos(vec.y), cos(vec.z));
	}
}