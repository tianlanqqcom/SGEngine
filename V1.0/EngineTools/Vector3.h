//
// Created by tianlan on 2023/4/18.
// 三维向量类，类似Unity中的Vector3
//

#ifndef SGENGINE_VECTOR3_H
#define SGENGINE_VECTOR3_H

#include <iostream>
#include "Matrix.h"

namespace EngineTools
{
    class Vector3
    {
    public:
        float x, y, z;

        Vector3() = default;

        Vector3(float _x, float _y, float _z);

        Vector3(const Vector3 &_vec);

        Vector3 operator+(const Vector3 &vec3) const;

        Vector3 operator-(const Vector3& vec3) const;

        float operator*(const Vector3 &vec3) const;

        Vector3 operator*(float time) const;

        Vector3 operator/(float time) const;


        [[nodiscard]] Vector3 Multiply(const Matrix& matrix) const;

        [[nodiscard]] std::string ToString() const;
    };


}
#endif //SGENGINE_VECTOR3_H
