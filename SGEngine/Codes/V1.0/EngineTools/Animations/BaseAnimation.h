//
// Created by tianlan on 2023/5/11.
// ��������
//

#ifndef SGENGINE_BASEANIMATION_H
#define SGENGINE_BASEANIMATION_H

#include "../GameObject.h"

namespace EngineTools
{

    class BaseAnimation
    {
    public:
        GameObject* gameObject = nullptr;   // Ŀ������

        virtual bool NextFrame();           // ִ����һ֡
        virtual void Restart();             // ��������
        virtual BaseAnimation* Copy();      // ��ȡ��������ĸ�����
    };

} // EngineTools

#endif //SGENGINE_BASEANIMATION_H
