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
        unitDirection = direction / time;   // 计算单位距离
        maxTime = time;
        timeRecord = 0;
    }

    void Translation::Reset(const Vector3& direction, float time)
    {
        unitDirection = direction / time;   // 计算单位距离
        maxTime = time;
        timeRecord = 0;
    }

    bool Translation::NextFrame()
    {
        // 物体移动
        gameObject->GetComponent<Transform>()->Translate(unitDirection * EngineTime::GetInstance()->deltaTimef);

        // 更新计时器
        timeRecord += EngineTime::GetInstance()->deltaTimef;

        // 如果动画结束，返回true
        if (timeRecord >= maxTime)
        {
            return true;
        }

        return false;
    }

    void Translation::Restart()
    {
        // 重启即直接清空计时器
        timeRecord = 0;
    }

    BaseAnimation* Translation::Copy()
    {
        // 返回一个与当前动画具有相同内容的动画对象，但未绑定物体
        return new Translation(unitDirection * maxTime, maxTime);
    }
} // EngineTools