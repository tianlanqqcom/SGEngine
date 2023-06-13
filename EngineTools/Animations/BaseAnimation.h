//
// Created by tianlan on 2023/5/11.
// 动画基类
//

#ifndef SGENGINE_BASEANIMATION_H
#define SGENGINE_BASEANIMATION_H

#include "../GameObject.h"

namespace EngineTools
{

    class BaseAnimation
    {
    public:
        GameObject* gameObject = nullptr;   // 目标物体

        virtual bool NextFrame();           // 执行下一帧
        virtual void Restart();             // 重启动画
        virtual BaseAnimation* Copy();      // 获取动画对象的复制体
    };

} // EngineTools

#endif //SGENGINE_BASEANIMATION_H
