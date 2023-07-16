//
// Created by tianlan on 2023/4/19.
// 矩阵类
// 警告：该文件暂未经过检验，安全性未知，建议使用glm代替
//

#ifndef SGENGINE_MATRIX_H
#define SGENGINE_MATRIX_H
#include <exception>
#include <string>
#include <cmath>
#include <cfloat>

namespace EngineTools
{
#define PI 3.14159265358797323846f
    class Matrix
    {
        unsigned int m = 1, n = 1;
        float **data = nullptr;
        float *ptr = nullptr;
        void init();

    public:
        Matrix();

        Matrix(unsigned int _m, unsigned int _n);

        Matrix(const Matrix &matrix);

        ~Matrix();

        Matrix &operator=(const Matrix &matrix);

        Matrix operator+(const Matrix &matrix);

        Matrix operator-(const Matrix &matrix);

        Matrix operator*(const Matrix &matrix);

        Matrix &operator+=(const Matrix &matrix);

        Matrix &operator-=(const Matrix &matrix);

        Matrix &operator*=(const Matrix &matrix);

        void SetValue(int x, int y, float val);

        [[nodiscard]] float GetValue(int x, int y) const;

        float *GetValuePointer();

        [[nodiscard]] std::string ToString() const;

        static Matrix GetUnitMatrix();

        static Matrix GetTranslateMatrix(float x,float y,float z);

        static Matrix GetRotationMatrix(float x,float y,float z);

        static Matrix GetOrthoOffCenterMatrix(float,float ,float ,float );

        static Matrix GetPerspectiveMatrix(float angle, float r, float near, float far);
    };

} // EngineTools

#endif //SGENGINE_MATRIX_H
