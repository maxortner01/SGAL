#pragma once

#include <math.h>
#include "Matrix.h"

namespace SGL
{
    template<typename T>
    struct Vec2
    {
        Vec2(T _x = 0, T _y = 0);

        T x, y;
    };

    template<typename T>
    struct Vec3
    {
        Vec3(T _x = 0, T _y = 0, T _z = 0);

        T x, y, z;

        template<typename U>
        Vec3<T> operator+(const Vec3<U>& vec) const
        {
            return Vec3<T>(x + vec.x, y + vec.y, z + vec.z);
        }
        
        template<typename U>
        Vec3<T> operator-(const Vec3<U>& vec) const
        {
            return Vec3<T>(x - vec.x, y - vec.y, z - vec.z);
        }

        template<typename U>
        Vec3<T> operator*(const U& scalar) const
        {
            return Vec3<T>(x * scalar, y * scalar, z * scalar);
        }
        
        template<typename U>
        Vec3<T> operator/(const U& scalar) const
        {
            return Vec3<T>(x / scalar, y / scalar, z / scalar);
        }

        template<typename U>
        void operator+=(const Vec3<U>& vec)
        {
            *this = (*this) + vec;
        }
        
        template<typename U>
        void operator-=(const Vec3<U>& vec)
        {
            *this = (*this) - vec;
        }
        
        template<typename U>
        void operator*=(const U& scalar)
        {
            *this = (*this) * scalar;
        }

        template<int C>
        Vec3<T> operator*(const Matrix<T, 3, C>& mat) const
        {
            Matrix<T, 1, 3> vec;
            vec(0, 0) = x; vec(0, 1) = y; vec(0, 2) = z;

            Matrix<T, 1, 3> result = vec * mat;
            return Vec3<T>(result(0, 0), result(0, 1), result(0, 2));
        }
    };

    template<typename T>
    struct Vec4
    {
        T x, y, z, w;

        Vec4(T _x, T _y, T _z, T _w);
    };

    template<typename T>
    Vec2<T>::Vec2(T _x, T _y) :
        x(_x), y(_y)
    {   }
    
    template<typename T>
    Vec3<T>::Vec3(T _x, T _y, T _z) :
        x(_x), y(_y), z(_z)
    {   }
    
    template<typename T>
    Vec4<T>::Vec4(T _x, T _y, T _z, T _w) :
        x(_x), y(_y), z(_z), w(_w)
    {   }
    
    typedef Vec2<int>           Vec2i;
    typedef Vec2<float>         Vec2f;
    typedef Vec2<double>        Vec2d;
    typedef Vec2<unsigned int>  Vec2u;

    typedef Vec3<int>           Vec3i;
    typedef Vec3<float>         Vec3f;
    typedef Vec3<double>        Vec3d;
    typedef Vec3<unsigned int>  Vec3u;

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
    
    static Mat4f makeTranslationMatrix(Vec3f position)
    {
        Mat4f ret;
        ret.toIdentity();

        ret(0, 3) = position.x;
        ret(1, 3) = position.y;
        ret(2, 3) = position.z;

        return ret;
    }

    static Mat4f makeRotationMatrix(Vec3f rotation)
    {
        Mat4f rot_x, rot_y, rot_z;
        rot_x.toIdentity();
        rot_y.toIdentity();
        rot_z.toIdentity();

        rot_x(1, 1) =  cos(rotation.x);
        rot_x(1, 2) = -sin(rotation.x);
        rot_x(2, 1) =  sin(rotation.x);
        rot_x(2, 2) =  cos(rotation.x);

        rot_y(0, 0) =  cos(rotation.y);
        rot_y(0, 2) =  sin(rotation.y);
        rot_y(2, 0) = -sin(rotation.y);
        rot_y(2, 2) =  cos(rotation.y);
        
        rot_z(0, 0) =  cos(rotation.z);
        rot_z(0, 1) = -sin(rotation.z);
        rot_z(1, 0) =  sin(rotation.z);
        rot_z(1, 1) =  cos(rotation.z);

        return rot_x * rot_y * rot_z;
    }

    static Mat4f makeScaleMatrix(Vec3f scale)
    {
        Mat4f ret;
        ret.toIdentity();

        ret(0, 0) = scale.x;
        ret(1, 1) = scale.y;
        ret(2, 2) = scale.z;

        return ret;
    }

    template<typename T, int N>
    static void getCofactor(const Matrix<T, N, N>& in, Matrix<T, N, N>& temp, int p, int q, int n)
    {
        int i = 0, j = 0;

        for (int row = 0; row < n; row++)
            for (int col = 0; col < n; col++)
                if (row != p && col != q)
                {
                    temp(i, j++) = in(row, col);

                    if (j == n - 1)
                    {
                        j = 0;
                        i++;
                    }
                }
    }

    template<typename T, int N>
    static double determinant(const Matrix<T, N, N>& matrix, int n)
    {
        double D = 0;

        if (n == 1) return (double)matrix(0, 0);

        Matrix<T, N, N> temp;
        double sign = 1.0;

        for (int f = 0; f < n; f++)
        {
            getCofactor(matrix, temp, 0, f, n);
            D += sign * matrix(0, f) * determinant(temp, n - 1);
            sign *= -1.0;
        }

        return D;
    }

    template<typename T, int N>
    static Matrix<T, N, N> adjoint(const Matrix<T, N, N>& A)
    {
        Matrix<T, N, N> adj;

        if (N == 1) 
        {
            adj(0, 0) = 1;
            return adj;
        }

        double sign = 1.0;
        Matrix<T, N, N> temp;

        for (int  i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
            {
                getCofactor(A, temp, i, j, N);
                sign = ((i + j) % 2 == 0) ? 1.0 : -1.0;
                adj(j, i) = sign * determinant(temp, N - 1);
            }

        return adj;
    }

    template<typename T, int N>
    static Matrix<T, N, N> inverse(const Matrix<T, N, N>& matrix)
    {
        double det = determinant(matrix, N);
        assert(det);

        Matrix<T, N, N> adj = adjoint(matrix);
        Matrix<T, N, N> inverse;

        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                inverse(i, j) = adj(i, j) / det;

        return inverse;
    }
    
    template<typename T, int N>
    static Matrix<T, N, N> transpose(const Matrix<T, N, N>& matrix)
    {
        Matrix<T, N, N> ret;

        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                ret(i, j) = matrix(j, i);

        return ret;
    }
}