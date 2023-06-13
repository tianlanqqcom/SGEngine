//
// Created by tianlan on 2023/4/23.
//

#include "EngineTime.h"

namespace EngineTools
{
    EngineTime *EngineTime::pInstance = nullptr; // ����ָ��Ĭ���ÿ�

    EngineTime *EngineTime::GetInstance()
    {
        // ���û�ж��󣬴�������ֵ��pInstance
        if (!pInstance)
        {
            pInstance = new EngineTime;
        }

        // ���ض���ָ��
        return pInstance;
    }

    void EngineTime::DeleteInstance()
    {
        // ������ڶ������ٲ���pInstance�ÿ�
        if (pInstance)
        {
            delete pInstance;
            pInstance = nullptr;
        }
    }

    EngineTime::EngineTime()
    {
        start = clock();        // ��ʼ����ʼʱ��
        lastRecordTime = start; // ��һ�μ�¼ʱ����ǿ�ʼʱ��
        nowTime = start;        // ��һ�ε�ǰʱ��Ҳ�ǿ�ʼʱ��
        deltaTime = 0;          // �ʼ��֡���Ϊ0
        deltaTimef = 0.0f;
    }

    void EngineTime::UpdateDeltaTime()
    {
        nowTime = clock();                      // ��ȡ��ǰϵͳʱ���
        deltaTime = nowTime - lastRecordTime;   // ֡���=��ǰʱ��-�ϴμ�¼ʱ��
        deltaTimef = deltaTime / 1000.0f;       // ���뻻�㵽��
        lastRecordTime = nowTime;               // ˢ�¼�¼��
    }
} // EngineTools