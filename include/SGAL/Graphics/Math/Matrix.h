/**
 * @file   Matrix.h
 * @author Max Ortner
 * @date   4-12-2020
 * 
 * Description...
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

#include <math.h>
#include <SGAL/decl.h>

#include "Vec.h"

namespace sgal
{
	template<typename T, int R, int C>
    class Matrix
    {
        T data[R][C];

    public:
        Matrix<T, R, C>()
        {
            for (int r = 0; r < R; r++)
                for (int c = 0; c < C; c++)
                    data[r][c] = 0;
        }

        void toIdentity()
        {
            for (int i = 0; i < R; i++)
                data[i][i] = 1;
        }

        template<typename T2, int C2>
        Matrix<T, R, C2> operator*(const Matrix<T2, C, C2>& in_mat)
        {
            Matrix<T, R, C2> mat;

            for (int r = 0; r < R; r++)
                for (int c = 0; c < C2; c++)
                    for (int j = 0; j < C; j++)
                        mat(r, c) += (*this)(r, j) * in_mat(j, c);

            return mat;
        }

        T& operator() (const unsigned int r, const unsigned int c)
        {
            return data[r][c];
        }

        T operator() (const unsigned int r, const unsigned int c) const
        {
            return data[r][c];
        }

        unsigned int getRows()    const { return R; }
        unsigned int getColumns() const { return C; }
    };

    typedef Matrix<float, 4, 4> Mat4f;
    typedef Matrix<float, 3, 3> Mat3f;

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
}