//
// Created by tianlan on 2023/4/23.
// 引擎时间，主要为每帧间隔提供支持
// 该类为单例类
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

        static EngineTime *pInstance;   // 指向当前有效的单例对象

        std::clock_t start;             // 开始时间
        std::clock_t lastRecordTime;    // 上一次刷新时记录的时间
        std::clock_t nowTime;           // 当前时间

    public:
        std::clock_t deltaTime;         // 每帧时间，以毫秒为单位
        float deltaTimef;               // 每帧时间，以秒为单位

        static EngineTime *GetInstance(); // 获取单例对象

        static void DeleteInstance();   // 清除对象

        void UpdateDeltaTime();         // 刷新每帧间隔
    };

} // EngineTools

#endif //SGENGINE_ENGINETIME_H
