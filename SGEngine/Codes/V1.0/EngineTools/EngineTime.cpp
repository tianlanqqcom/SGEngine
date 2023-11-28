//
// Created by tianlan on 2023/4/23.
//

#include "EngineTime.h"

namespace EngineTools
{
    EngineTime *EngineTime::pInstance = nullptr; // 对象指针默认置空

    EngineTime *EngineTime::GetInstance()
    {
        // 如果没有对象，创建并赋值给pInstance
        if (!pInstance)
        {
            pInstance = new EngineTime;
        }

        // 返回对象指针
        return pInstance;
    }

    void EngineTime::DeleteInstance()
    {
        // 如果存在对象，销毁并把pInstance置空
        if (pInstance)
        {
            delete pInstance;
            pInstance = nullptr;
        }
    }

    EngineTime::EngineTime()
    {
        start = clock();        // 初始化开始时间
        lastRecordTime = start; // 第一次记录时间就是开始时间
        nowTime = start;        // 第一次当前时间也是开始时间
        deltaTime = 0;          // 最开始的帧间隔为0
        deltaTimef = 0.0f;
    }

    void EngineTime::UpdateDeltaTime()
    {
        nowTime = clock();                      // 获取当前系统时间戳
        deltaTime = nowTime - lastRecordTime;   // 帧间隔=当前时间-上次记录时间
        deltaTimef = deltaTime / 1000.0f;       // 毫秒换算到秒
        lastRecordTime = nowTime;               // 刷新记录点
    }
} // EngineTools