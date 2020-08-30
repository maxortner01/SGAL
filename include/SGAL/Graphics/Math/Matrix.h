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

#include "Instrins.h"
#include "Vec.h"

namespace sgal
{
    /**
     * @brief Represents a matrix of information used in mathematical transforms.
     * 
     * @tparam T Data type
     * @tparam R Rows
     * @tparam C Columns
     */
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
        Matrix<T, R, C2> operator*(const Matrix<T2, C, C2>& in_mat) const
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

    typedef Matrix<float,  4, 4> Mat4f;
    typedef Matrix<float,  3, 3> Mat3f;
    
    typedef Matrix<float,  2, 2> Mat2f;
    typedef Matrix<double, 2, 2> Mat2d;

	static Mat4f makeTranslationMatrix(Vec3f position)
    {
        Mat4f ret;
        ret.toIdentity();

        ret(0, 3) = position.x;
        ret(1, 3) = position.y;
        ret(2, 3) = position.z;

        return ret;
    }
    VA_FUNCTION(makeTranslationMatrix, static Mat4f, Vec3f);

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
    VA_FUNCTION(makeRotationMatrix, static Mat4f, Vec3f);

    static Mat4f makeScaleMatrix(Vec3f scale)
    {
        Mat4f ret;
        ret.toIdentity();

        ret(0, 0) = scale.x;
        ret(1, 1) = scale.y;
        ret(2, 2) = scale.z;

        return ret;
    }
    VA_FUNCTION(makeScaleMatrix, static Mat4f, Vec3f);

    #define sgMatT(...) makeTranslationMatrix(__VA_ARGS__)
    #define sgMatR(...) makeRotationMatrix(__VA_ARGS__)
    #define sgMatS(...) makeScaleMatrix(__VA_ARGS__)
}