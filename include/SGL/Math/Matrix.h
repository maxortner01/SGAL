#pragma once

#include <cassert>

namespace SGL
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
            assert(R == C);
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
}