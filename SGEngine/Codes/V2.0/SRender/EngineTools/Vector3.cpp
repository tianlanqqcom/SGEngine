#include "Vector3.h"

namespace EngineTools
{

    Vector3::Vector3(double _x, double _y, double _z) : x(_x), y(_y), z(_z)
    {}

    Vector3::Vector3(const Vector3 &_vec)
    {
        x = _vec.x;
        y = _vec.y;
        z = _vec.z;
    }

    Vector3 Vector3::operator+(const Vector3 &vec3) const
    {
        return {x + vec3.x, y + vec3.y, z + vec3.z};
    }

    Vector3 Vector3::operator-(const Vector3& vec3) const
    {
        return { x - vec3.x, y - vec3.y, z - vec3.z };
    }

    double Vector3::operator*(const Vector3 &vec3) const
    {
        return x * vec3.x + y * vec3.y + z * vec3.z;
    }

    Vector3 Vector3::Multiply(const Matrix &matrix) const
    {
        Matrix vec(1, 4);
        vec.SetValue(0, 0, x);
        vec.SetValue(0, 1, y);
        vec.SetValue(0, 2, z);
        vec.SetValue(0, 3, 1);

        vec *= matrix;

        return {vec.GetValue(0, 0), vec.GetValue(0, 1), vec.GetValue(0, 2)};
    }

    std::string Vector3::ToString() const
    {
        return "Vector3(" + std::to_string(x) + ' '
               + std::to_string(y) + ' ' + std::to_string(z) + ")\n";
    }

    Vector3& Vector3::Normalize()
    {
        double len = sqrt(x * x + y * y + z * z);
        x /= len;
        y /= len;
        z /= len;
        return *this;
    }

    double Vector3::Dot(const Vector3& other) const
    {
        return x * other.x + y * other.y + z * other.z;
    }

    Vector3 Vector3::Cross(const Vector3& other) const
    {
        return { y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x };
    }

    double Vector3::Distance(const Vector3& other) const
    {
        return sqrt((x - other.x) * (x - other.x) +
            (y - other.y) * (y - other.y) +
            (z - other.z) * (z - other.z));
    }

    Vector3 Vector3::operator/(double time) const
    {
        return {x / time, y / time, z / time};
    }

    Vector3 Vector3::operator*(double time) const
    {
        return {x * time, y * time, z * time};
    }
}