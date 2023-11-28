//
// Created by tianlan on 2023/4/23.
// ����ʱ�䣬��ҪΪÿ֡����ṩ֧��
// ����Ϊ������
//

#ifndef SGENGINE_ENGINETIME_H
#define SGENGINE_ENGINETIME_H

#include <ctime>
#include <Windows.h>
#include <synchapi.h>

namespace EngineTools
{

    class EngineTime
    {
        EngineTime();

        ~EngineTime() = default;

        static EngineTime *pInstance;   // ָ��ǰ��Ч�ĵ�������

        std::clock_t start;             // ��ʼʱ��
        std::clock_t lastRecordTime;    // ��һ��ˢ��ʱ��¼��ʱ��
        std::clock_t nowTime;           // ��ǰʱ��

    public:
        std::clock_t deltaTime;         // ÿ֡ʱ�䣬�Ժ���Ϊ��λ
        float deltaTimef;               // ÿ֡ʱ�䣬����Ϊ��λ

        static EngineTime *GetInstance(); // ��ȡ��������

        static void DeleteInstance();   // �������

        void UpdateDeltaTime();         // ˢ��ÿ֡���
    };

} // EngineTools

#endif //SGENGINE_ENGINETIME_H
