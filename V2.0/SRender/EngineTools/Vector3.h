//
// Created by tianlan on 2023/4/18.
// 三维向量类，类似Unity中的Vector3
//

#ifndef SGENGINE_VECTOR3_H
#define SGENGINE_VECTOR3_H

#include <iostream>
#include <cmath>
#include "Matrix.h"

namespace EngineTools
{
    class Vector3
    {
    public:
        double x, y, z;

        Vector3() = default;

        Vector3(double _x, double _y, double _z);

        Vector3(const Vector3 &_vec);

        Vector3 operator+(const Vector3 &vec3) const;

        Vector3 operator-(const Vector3& vec3) const;

        double operator*(const Vector3 &vec3) const;

        Vector3 operator*(double time) const;

        Vector3 operator/(double time) const;


        [[nodiscard]] Vector3 Multiply(const Matrix& matrix) const;

        [[nodiscard]] std::string ToString() const;

        Vector3& Normalize();

        double Dot(const Vector3& other) const;

        Vector3 Cross(const Vector3& other) const;

        double Distance(const Vector3& other) const;
    };


}
#endif //SGENGINE_VECTOR3_H
