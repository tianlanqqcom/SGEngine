//
// Created by tianlan on 2023/4/19.
//

#include "Matrix.h"
#include "Vector3.h"
#include <cmath>

using namespace std;

namespace EngineTools
{
    void Matrix::init()
    {
        data = new float *[m];
        for (int i = 0; i < m; ++i)
        {
            data[i] = new float[n];
            for (int j = 0; j < n; ++j)
            {
                data[i][j] = 0;
            }
        }
    }

    Matrix::Matrix()
    {
        init();
    }

    Matrix::Matrix(unsigned int _m, unsigned int _n) : m(_m), n(_n)
    {
        init();
    }

    Matrix::~Matrix()
    {
        for (int i = 0; i < m; ++i)
        {
            delete[] data[i];
        }
        delete[] data;
    }

    Matrix &Matrix::operator=(const Matrix &matrix)
    {
        if (this == &matrix)
        {
            return *this;
        }

        if (m != matrix.m || n != matrix.n)
        {
            for (int i = 0; i < m; ++i)
            {
                delete[] data[i];
            }
            delete[] data;

            m = matrix.m;
            n = matrix.n;
            init();
        }

        for (int i = 0; i < m; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                data[i][j] = matrix.data[i][j];
            }
        }

        return *this;
    }

    Matrix::Matrix(const Matrix &matrix)
    {
        if (data)
        {
            for (int i = 0; i < m; ++i)
            {
                delete[] data[i];
            }
        }

        m = matrix.m;
        n = matrix.n;
        init();

        for (int i = 0; i < m; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                data[i][j] = matrix.data[i][j];
            }
        }
    }

    Matrix Matrix::operator+(const Matrix &matrix)
    {
        if (m != matrix.m || n != matrix.n)
        {
            throw std::exception();
        }

        Matrix temp = *this;
        for (int i = 0; i < m; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                temp.data[i][j] += matrix.data[i][j];
            }
        }
        return temp;
    }

    Matrix Matrix::operator-(const Matrix &matrix)
    {
        if (m != matrix.m || n != matrix.n)
        {
            throw std::exception();
        }

        Matrix temp = *this;
        for (int i = 0; i < m; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                temp.data[i][j] -= matrix.data[i][j];
            }
        }
        return temp;
    }

    Matrix Matrix::operator*(const Matrix &matrix)
    {
        if (n != matrix.m)
        {
            throw std::exception();
        }

        Matrix temp(m, matrix.n);
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < matrix.n; j++)
            {
                for (int k = 0; k < n; k++)
                {
                    temp.data[i][j] += (data[i][k] * matrix.data[k][j]);
                }
            }
        }
        return temp;
    }

    Matrix &Matrix::operator+=(const Matrix &matrix)
    {
        *this = *this + matrix;
        return *this;
    }

    Matrix &Matrix::operator-=(const Matrix &matrix)
    {
        *this = *this - matrix;
        return *this;
    }

    Matrix &Matrix::operator*=(const Matrix &matrix)
    {
        *this = *this * matrix;
        return *this;
    }

    void Matrix::SetValue(int x, int y, float val)
    {
        data[x][y] = val;
    }

    float Matrix::GetValue(int x, int y) const
    {
        return data[x][y];
    }

    std::string Matrix::ToString() const
    {
        std::string res;
        if (data)
        {
            for (int i = 0; i < m; ++i)
            {
                for (int j = 0; j < n; ++j)
                {
                    res += std::to_string(data[i][j]);
                    res += ' ';
                }
                res += '\n';
            }
        }
        else
        {
            res = "NULL";
        }
        return res;
    }

    Matrix Matrix::GetRotationMatrix(float x, float y, float z)
    {
        Vector3 direction(x, y, z);
        Vector3 angle{direction.x * PI / 180, direction.y * PI / 180, direction.z * PI / 180};
        Matrix res(4, 4);
//        Matrix ry(4, 4);
//        Matrix rx(4, 4);
//        Matrix rz(4, 4);
//
//        ry.SetValue(0, 0, std::cos(angle.y));
//        ry.SetValue(0, 2, std::sin(angle.y));
//        ry.SetValue(2, 0, -std::sin(angle.y));
//        ry.SetValue(2, 2, std::cos(angle.y));
//        ry.SetValue(1, 1, 1);
//        ry.SetValue(3, 3, 1);
//
//        rx.SetValue(1, 1, std::cos(angle.y));
//        rx.SetValue(1, 2, -std::sin(angle.y));
//        rx.SetValue(2, 1, std::sin(angle.y));
//        rx.SetValue(2, 2, std::cos(angle.y));
//        rx.SetValue(0, 0, 1);
//        rx.SetValue(3, 3, 1);
//
//        rz.SetValue(0, 0, std::cos(angle.z));
//        rz.SetValue(0, 1, -std::sin(angle.z));
//        rz.SetValue(1, 0, std::sin(angle.z));
//        rz.SetValue(1, 1, std::cos(angle.z));
//        rz.SetValue(2, 2, 1);
//        rz.SetValue(3, 3, 1);

        // res = ry * rx * rz;

//        res.SetValue(0, 0, std::cos(angle.x) * cos(angle.z)
//                           - cos(angle.y) * sin(angle.x) * sin((angle.z)));
//        res.SetValue(0, 1, -cos(angle.y) * cos(angle.z) * sin(angle.x)
//                           - cos(angle.x) * sin(angle.z));
//        res.SetValue(0, 2, sin(angle.x) * sin(angle.y));
//        res.SetValue(1, 0, cos(angle.z) * sin(angle.x)
//                           + cos(angle.x) * cos(angle.y) * sin(angle.z));
//        res.SetValue(1, 1, cos(angle.x) * cos(angle.y) * cos(angle.z)
//                           - sin(angle.x) * sin(angle.z));
//        res.SetValue(1, 2, -cos(angle.x) * sin(angle.y));
//        res.SetValue(2, 0, sin(angle.y) * sin(angle.z));
//        res.SetValue(2, 1, cos(angle.z) * sin(angle.y));
//        res.SetValue(2, 2, cos(angle.y));
//        res.SetValue(3, 3, 1);

        res.SetValue(2, 2, std::cos(angle.x) * cos(angle.z)
                           - cos(angle.y) * sin(angle.x) * sin((angle.z)));
        res.SetValue(2, 1, -cos(angle.y) * cos(angle.z) * sin(angle.x)
                           - cos(angle.x) * sin(angle.z));
        res.SetValue(2, 0, sin(angle.x) * sin(angle.y));
        res.SetValue(1, 2, cos(angle.z) * sin(angle.x)
                           + cos(angle.x) * cos(angle.y) * sin(angle.z));
        res.SetValue(1, 1, cos(angle.x) * cos(angle.y) * cos(angle.z)
                           - sin(angle.x) * sin(angle.z));
        res.SetValue(1, 0, -cos(angle.x) * sin(angle.y));
        res.SetValue(0, 2, sin(angle.y) * sin(angle.z));
        res.SetValue(0, 1, cos(angle.z) * sin(angle.y));
        res.SetValue(0, 0, cos(angle.y));
        res.SetValue(3, 3, 1);
        return res;
    }

    Matrix Matrix::GetTranslateMatrix(float x, float y, float z)
    {
        Matrix res(4, 4);
        //res.SetValue(0, 0, 1);
        //res.SetValue(1, 1, 1);
        //res.SetValue(2, 2, 1);
        //res.SetValue(3, 3, 1);
        res.SetValue(3, 0, x);
        res.SetValue(3, 1, y);
        res.SetValue(3, 2, z);
        return res;
    }

    Matrix Matrix::GetPerspectiveMatrix(float angle, float r, float near, float far)
    {
        float ang = angle * PI / 180;
        Matrix res(4, 4);

        float ag_2_tan = tan(ang / 2);
        float ag_2_cot;
        if (ang == 0)
        {
            ag_2_cot = FLT_MAX;
        }
        else
        {
            ag_2_cot = 1 / ag_2_tan;
        }
        res.SetValue(0, 0, 1 / r * ag_2_cot);
        res.SetValue(1, 1, ag_2_cot);
        res.SetValue(2, 2, -(far + near) / (far - near));
        res.SetValue(2, 3, -1);
        res.SetValue(3, 2, -2 * (far * near) / (far - near));

        return res;
    }

    float *Matrix::GetValuePointer()
    {
        delete[] ptr;
        ptr = new float[m * n];
        int pointer = 0;
        for (int i = 0; i < m; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                ptr[pointer++] = data[i][j];
            }
        }
        return ptr;
    }

    Matrix Matrix::GetUnitMatrix()
    {
        Matrix res(4,4);
        res.SetValue(0, 0, 1);
        res.SetValue(1, 1, 1);
        res.SetValue(2, 2, 1);
        res.SetValue(3, 3, 1);
        return res;
    }


} // EngineTools