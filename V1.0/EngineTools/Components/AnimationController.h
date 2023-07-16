//
// Created by tianlan on 2023/5/11.
// 动画控制器，控制场景中动画的进行
//

#ifndef SGENGINE_ANIMATIONCONTROLLER_H
#define SGENGINE_ANIMATIONCONTROLLER_H
#include <vector>
#include "../Component.h"
#include "../Animation.h"
#include "../ActionSequence.h"

namespace EngineTools
{

    class AnimationController: public Component
    {
        std::vector<ActionSequence*> actions;   // 动画序列列表
    public:
        AnimationController();

        // 向控制器中添加序列
        // @ para ActionSequence* sequence  将要添加的序列的指针
        void AddSequence(ActionSequence* sequence);

        void Update() override;

        std::string ToString(int depth = 0) const override;
    };

} // EngineTools

#endif //SGENGINE_ANIMATIONCONTROLLER_H
