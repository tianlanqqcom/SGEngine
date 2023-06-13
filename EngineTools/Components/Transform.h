//
// Created by tianlan on 2023/4/18.
// Transform�������Unity�е�Transform����
//

#ifndef SGENGINE_TRANSFORM_H
#define SGENGINE_TRANSFORM_H

#include "../Component.h"
#include "../Vector3.h"

namespace EngineTools
{
    class Transform : public Component
    {
    public:
        Vector3 localPosition{};    // �Ը�����λ��Ϊԭ������λ��
        Vector3 worldPosition{};    // ����λ��
        // ��ת�Ƕȣ�ע�⣺����������תʱ��Ӱ�����������ת�������Ҫ������ת������������ʹ��RotationWithRoot����
        Vector3 rotation{};         
        Vector3 scale{1, 1, 1};     // ���ţ�ͬ�ϣ����������Ų�Ӱ��������

        Transform();

        void OnValidate() override; // ȷ�����������Ƿ����
        void Awake() override{};    // δʹ��
        void OnEnable() override{}; // δʹ��
        void Start() override;      // ȷ�����������Ƿ����
        void Update() override;     // ȷ�����������Ƿ����
        void OnDisable() override{};// δʹ��
        void OnRemove() override{}; // δʹ��

        // ���������ƽ�Ʊ任
        // @ para const Vector3& vec3   �ƶ��ķ��򼰾���
        void Translate(const Vector3& vec3);    

        // ��ת����
        // @ para const Vector3& vec3 ��ת�Ƕȣ��Զ�Ϊ��λ
        void Rotation(const Vector3& vec3);

        // ���游������ת
        // @ para const Vector3& vec3 ��ת�Ƕȣ��Զ�Ϊ��λ
        // @ para bool changeSelfRotation = true    �Ƿ�ı䵱ǰ������ת�Ƕȣ����false���������λ�ö����ı���ת�Ƕ�
        void RotationWithRoot(const Vector3& vec3, bool changeSelfRotation = true);

        // ToString��������ʾ���ݳ�Ա
        std::string ToString(int depth = 0) const override;
    };

} // EngineTools

#endif //SGENGINE_TRANSFORM_H
