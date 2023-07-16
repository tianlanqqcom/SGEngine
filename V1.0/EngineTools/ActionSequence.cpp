//
// Created by tianlan on 2023/5/11.
//

#include "ActionSequence.h"

namespace EngineTools
{
    std::vector<BaseAnimation *>
    ActionSequence::MakeParallelAnimations(std::initializer_list<BaseAnimation *> animations)
    {   
        return animations;
    }

    void ActionSequence::AddAnimation(const std::vector<BaseAnimation *> &animation, unsigned int time)
    {
        // 如果是无尽循环
        if (time == ENDLESS_LOOP)
        {
            std::vector<BaseAnimation*> newAnimation;

            // 把当前动画项的复制体加入到列表中
            for (auto& ani : animation)
            {
                newAnimation.emplace_back(ani->Copy());
            }
            actions.emplace_back(newAnimation);

            // 设置无尽循环标记
            hasEndless = true;
            markEndless = actions.size() - 1;
        }
        else
        {
            // 进行time次操作
            for (unsigned int i = 0; i < time; ++i)
            {
                std::vector<BaseAnimation*> newAnimation;

                // 把当前动画项的复制体加入到列表中
                for (auto& ani : animation)
                {
                    newAnimation.emplace_back(ani->Copy());
                }
                actions.emplace_back(newAnimation);
            }
        }
    }

    void ActionSequence::BindWith(GameObject *object)
    {
        for (const auto &vector: actions)
        {
            for (auto animation: vector)
            {
                animation->gameObject = object;
            }
        }
    }

    bool ActionSequence::RunAction()
    {
        bool animationEnd = false;
        for (auto animations: actions[nowRunning])
        {
            animationEnd = animations->NextFrame();
        }

        if (hasEndless && animationEnd && nowRunning == markEndless)
        {
            for(auto animations: actions[nowRunning]){
                animations->Restart();
            }
            return false;
        }
        else if (!hasEndless && animationEnd && nowRunning == actions.size() - 1)
        {
            return true;
        }
        else if (animationEnd)
        {
            ++nowRunning;
            return false;
        }
        else
        {
            return false;
        }
    }
} // EngineTools