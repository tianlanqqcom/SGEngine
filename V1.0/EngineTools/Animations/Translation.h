//
// Created by tianlan on 2023/5/11.
// 平移动画
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
        Vector3 unitDirection{0, 0, 0}; // 每秒应该移动的向量
        float timeRecord = 0.0f;        // 运行时间记录
        float maxTime = 0.0f;           // 最大时间
    public:
        Translation() = default;        // 默认构造函数

        // 构造函数
        // @ para const Vector3& direction  移动的总距离向量
        // @ para float time                动画时长
        Translation(const Vector3& direction, float time);

        // 重新设置动画信息
        // @ para const Vector3& direction  移动的总距离向量
        // @ para float time                动画时长
        void Reset(const Vector3& direction, float time);

        // 执行下一帧
        bool NextFrame() override;

        // 重新开始动画
        void Restart() override;

        // 得到当前动画的复制体
        BaseAnimation* Copy() override;
    };

} // EngineTools

#endif //SGENGINE_TRANSLATION_H
