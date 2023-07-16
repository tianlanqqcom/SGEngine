//
// Created by tianlan on 2023/5/11.
// 动画序列
//

#ifndef SGENGINE_ACTIONSEQUENCE_H
#define SGENGINE_ACTIONSEQUENCE_H

#include <vector>
#include <initializer_list>
#include "Animation.h"

namespace EngineTools
{
#define ENDLESS_LOOP 0xffffffff // 无尽循环
    class ActionSequence
    {
    private:
        std::vector<std::vector<BaseAnimation*>> actions;   // 所有的动画，允许平行动画
        bool hasEndless = false;    // 标记该序列里是否有无尽动画
        size_t markEndless = -1;    // 标记无尽动画的位置，如果没有则为-1
        size_t nowRunning = 0;      // 标记当前正在运行的动画索引值
    public:
        ActionSequence()= default;  // 默认构造函数

        // 制作一个平行动画序列，要求用于制作的这些动画时长一样
        // @ para std::initializer_list<BaseAnimation*> animations 需要并列运行的动画们，以{...}的形式传入
        // @ ret  std::vector<BaseAnimation*>           制作好的平行序列
        static std::vector<BaseAnimation*> MakeParallelAnimations(std::initializer_list<BaseAnimation*> animations);

        // 向序列中添加动画项，并设置重复次数
        // @ para const std::vector<BaseAnimation*>& animation  动画项
        // @ para unsigned int time = 1     循环次数，默认为1
        void AddAnimation(const std::vector<BaseAnimation*>& animation, unsigned int time = 1);

        // 将动画序列与物体绑定
        // @ para GameObject* object    目标物体指针
        void BindWith(GameObject* object);

        // 运行动画，每次调用往下走一帧
        // @ ret bool 如果动画序列结束返回true
        bool RunAction();
    };

} // EngineTools

#endif //SGENGINE_ACTIONSEQUENCE_H
