//
// Created by tianlan on 2023/5/11.
// ƽ�ƶ���
//

#ifndef SGENGINE_TRANSLATION_H
#define SGENGINE_TRANSLATION_H

#include "BaseAnimation.h"
#include "../Vector3.h"

namespace EngineTools
{

    class Translation : public BaseAnimation
    {
    private:
        Vector3 unitDirection{0, 0, 0}; // ÿ��Ӧ���ƶ�������
        float timeRecord = 0.0f;        // ����ʱ���¼
        float maxTime = 0.0f;           // ���ʱ��
    public:
        Translation() = default;        // Ĭ�Ϲ��캯��

        // ���캯��
        // @ para const Vector3& direction  �ƶ����ܾ�������
        // @ para float time                ����ʱ��
        Translation(const Vector3& direction, float time);

        // �������ö�����Ϣ
        // @ para const Vector3& direction  �ƶ����ܾ�������
        // @ para float time                ����ʱ��
        void Reset(const Vector3& direction, float time);

        // ִ����һ֡
        bool NextFrame() override;

        // ���¿�ʼ����
        void Restart() override;

        // �õ���ǰ�����ĸ�����
        BaseAnimation* Copy() override;
    };

} // EngineTools

#endif //SGENGINE_TRANSLATION_H
