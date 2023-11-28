//
// Created by tianlan on 2023/4/18.
//
#include "../GameObject.h"
#include "Transform.h"

namespace EngineTools
{
    Transform::Transform()
    {
        typeHashCode = typeid(Transform).hash_code();   // 更新组件哈希码
    }

    void Transform::OnValidate()
    {
        Component::OnValidate();

        // 如果有父物体且父物体有Transform组件，世界坐标=局部坐标+父物体的世界坐标
        if (gameObject->parent && gameObject->parent->GetComponent<Transform>())
        {
            worldPosition = localPosition + gameObject->parent->GetComponent<Transform>()->worldPosition;
        }
        // 如果没有，世界坐标=当前局部坐标
        else
        {
            worldPosition = localPosition;
        }
    }

    void Transform::Translate(const Vector3 &vec3)
    {
        localPosition = localPosition + vec3;   // 更新局部坐标
        worldPosition = worldPosition + vec3;   // 更新世界坐标
    }

    void Transform::Rotation(const Vector3 &vec3)
    {
        rotation = rotation + vec3;     // 更新旋转角度
    }

    void Transform::RotationWithRoot(const Vector3& vec3, bool changeSelfRotation)
    {
        // 角度转换为弧度
		Vector3 angle = { 
            vec3.x * 0.01745329251994329576923690768489f,
		    vec3.y * 0.01745329251994329576923690768489f,
		    vec3.z * 0.01745329251994329576923690768489f
		};

        // 更新局部坐标
        localPosition = {
            cos(angle.y) * localPosition.x + sin(angle.y) * localPosition.z,
            localPosition.y,
            -sin(angle.y)* localPosition.x + cos(angle.y) * localPosition.z,
        };

        localPosition = {
            localPosition.x,
            cos(angle.x) * localPosition.y - sin(angle.x) * localPosition.z,
            sin(angle.x) * localPosition.y + cos(angle.x) * localPosition.z,
        };

        localPosition = {
            cos(angle.z) * localPosition.x - sin(angle.z) * localPosition.y,
            sin(angle.z) * localPosition.x + cos(angle.z) * localPosition.y,
            localPosition.z,
        };

        // 旋转自身
        if (changeSelfRotation)
        {
            rotation = rotation + vec3;
        }

        // 更新世界坐标
        OnValidate();
    }

    SRender::MathTools::Matrix<double> Transform::GetWorldModelMatrix() const
    {
        SRender::MathTools::Matrix<double> res(4, 4, 0);

        // Get Unit Matrix
        res[0][0] = 1;
        res[1][1] = 1;
        res[2][2] = 1;
        res[3][3] = 1;

        // Translate
        res[0][3] = this->worldPosition.x;
        res[1][3] = this->worldPosition.y;
        res[2][3] = this->worldPosition.z;

        // Rotation: Y - X - Z
        SRender::MathTools::Matrix<double> rotationY(4, 4, 0);
        rotationY[3][3] = 1;

        rotationY[0][0] = cos(this->rotation.y * 0.01745329251994329576923690768489);
        rotationY[0][2] = sin(this->rotation.y * 0.01745329251994329576923690768489);
        rotationY[1][1] = 1;
        rotationY[2][0] = -rotationY[0][2];
        rotationY[2][2] = rotationY[0][0];

        res *= rotationY;

        SRender::MathTools::Matrix<double> rotationX(4, 4, 0);
        rotationX[3][3] = 1;

        rotationX[0][0] = 1;
        rotationX[1][1] = cos(this->rotation.x * 0.01745329251994329576923690768489);
        rotationX[1][2] = -sin(this->rotation.x * 0.01745329251994329576923690768489);
        rotationX[2][1] = -rotationX[1][2];
        rotationX[2][2] = rotationX[1][1];

        res *= rotationX;

        SRender::MathTools::Matrix<double> rotationZ(4, 4, 0);
        rotationZ[3][3] = 1;

        rotationZ[0][0] = cos(this->rotation.z * 0.01745329251994329576923690768489);
        rotationZ[0][1] = -sin(this->rotation.z * 0.01745329251994329576923690768489);
        rotationZ[1][0] = -rotationZ[0][1];
        rotationZ[1][1] = rotationZ[0][0];
        rotationZ[2][2] = 1;

        res *= rotationZ;

        // Scale
        SRender::MathTools::Matrix<double> scaleMat(4, 4, 0);
        scaleMat[0][0] = this->scale.x;
        scaleMat[1][1] = this->scale.y;
        scaleMat[2][2] = this->scale.z;
        scaleMat[3][3] = 1;

        res *= scaleMat;

        return res;
    }

    std::string Transform::ToString(int depth) const
    {
        std::string ret = "";   // 返回值

        // 输出组件名，depth用于控制前面的空格个数
        for (int i = 0; i < depth; i++)
        {
            ret += ' ';
        }
        ret += "-Component:Transform\n";

        // 输出局部坐标
        for (int i = 0; i < depth; i++)
        {
            ret += ' ';
        }
        ret += "@localPosition: ";
        ret += this->localPosition.ToString();

        // 输出世界坐标
        for (int i = 0; i < depth; i++)
        {
            ret += ' ';
        }
        ret += "@worldPosition: ";
        ret += this->worldPosition.ToString();

        // 输出旋转角度
        for (int i = 0; i < depth; i++)
        {
            ret += ' ';
        }
        ret += "@rotation: ";
        ret += this->rotation.ToString();

        // 输出缩放
        for (int i = 0; i < depth; i++)
        {
            ret += ' ';
        }
        ret += "@scale: ";
        ret += this->scale.ToString();

        // 返回拼接好的字符串
        return ret;
    }

    void Transform::Update()
    {
        Component::Update();
        OnValidate();
    }

    void Transform::Start()
    {
        Component::Start();
        OnValidate();
    }
} // EngineTools