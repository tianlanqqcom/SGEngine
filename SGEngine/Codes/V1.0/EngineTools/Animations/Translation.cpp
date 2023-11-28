//
// Created by tianlan on 2023/5/11.
//

#include "Translation.h"
#include "../Components/Transform.h"
#include "../EngineTime.h"

namespace EngineTools
{
    Translation::Translation(const Vector3 &direction, float time)
    {
        unitDirection = direction / time;   // ���㵥λ����
        maxTime = time;
        timeRecord = 0;
    }

    void Translation::Reset(const Vector3& direction, float time)
    {
        unitDirection = direction / time;   // ���㵥λ����
        maxTime = time;
        timeRecord = 0;
    }

    bool Translation::NextFrame()
    {
        // �����ƶ�
        gameObject->GetComponent<Transform>()->Translate(unitDirection * EngineTime::GetInstance()->deltaTimef);

        // ���¼�ʱ��
        timeRecord += EngineTime::GetInstance()->deltaTimef;

        // �����������������true
        if (timeRecord >= maxTime)
        {
            return true;
        }

        return false;
    }

    void Translation::Restart()
    {
        // ������ֱ����ռ�ʱ��
        timeRecord = 0;
    }

    BaseAnimation* Translation::Copy()
    {
        // ����һ���뵱ǰ����������ͬ���ݵĶ������󣬵�δ������
        return new Translation(unitDirection * maxTime, maxTime);
    }
} // EngineTools