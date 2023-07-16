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
        // ������޾�ѭ��
        if (time == ENDLESS_LOOP)
        {
            std::vector<BaseAnimation*> newAnimation;

            // �ѵ�ǰ������ĸ�������뵽�б���
            for (auto& ani : animation)
            {
                newAnimation.emplace_back(ani->Copy());
            }
            actions.emplace_back(newAnimation);

            // �����޾�ѭ�����
            hasEndless = true;
            markEndless = actions.size() - 1;
        }
        else
        {
            // ����time�β���
            for (unsigned int i = 0; i < time; ++i)
            {
                std::vector<BaseAnimation*> newAnimation;

                // �ѵ�ǰ������ĸ�������뵽�б���
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