//
// Created by tianlan on 2023/4/18.
//
#include "../GameObject.h"
#include "Transform.h"

namespace EngineTools
{
    Transform::Transform()
    {
        typeHashCode = typeid(Transform).hash_code();   // ���������ϣ��
    }

    void Transform::OnValidate()
    {
        Component::OnValidate();

        // ����и������Ҹ�������Transform�������������=�ֲ�����+���������������
        if (gameObject->parent && gameObject->parent->GetComponent<Transform>())
        {
            worldPosition = localPosition + gameObject->parent->GetComponent<Transform>()->worldPosition;
        }
        // ���û�У���������=��ǰ�ֲ�����
        else
        {
            worldPosition = localPosition;
        }
    }

    void Transform::Translate(const Vector3 &vec3)
    {
        localPosition = localPosition + vec3;   // ���¾ֲ�����
        worldPosition = worldPosition + vec3;   // ������������
    }

    void Transform::Rotation(const Vector3 &vec3)
    {
        rotation = rotation + vec3;     // ������ת�Ƕ�
    }

    void Transform::RotationWithRoot(const Vector3& vec3, bool changeSelfRotation)
    {
        // �Ƕ�ת��Ϊ����
		Vector3 angle = { 
            vec3.x * 0.01745329251994329576923690768489f,
		    vec3.y * 0.01745329251994329576923690768489f,
		    vec3.z * 0.01745329251994329576923690768489f
		};

        // ���¾ֲ�����
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

        // ��ת����
        if (changeSelfRotation)
        {
            rotation = rotation + vec3;
        }

        // ������������
        OnValidate();
    }

    std::string Transform::ToString(int depth) const
    {
        std::string ret = "";   // ����ֵ

        // ����������depth���ڿ���ǰ��Ŀո����
        for (int i = 0; i < depth; i++)
        {
            ret += ' ';
        }
        ret += "-Component:Transform\n";

        // ����ֲ�����
        for (int i = 0; i < depth; i++)
        {
            ret += ' ';
        }
        ret += "@localPosition: ";
        ret += this->localPosition.ToString();

        // �����������
        for (int i = 0; i < depth; i++)
        {
            ret += ' ';
        }
        ret += "@worldPosition: ";
        ret += this->worldPosition.ToString();

        // �����ת�Ƕ�
        for (int i = 0; i < depth; i++)
        {
            ret += ' ';
        }
        ret += "@rotation: ";
        ret += this->rotation.ToString();

        // �������
        for (int i = 0; i < depth; i++)
        {
            ret += ' ';
        }
        ret += "@scale: ";
        ret += this->scale.ToString();

        // ����ƴ�Ӻõ��ַ���
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