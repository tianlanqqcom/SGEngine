//
// Created by tianlan on 2023/5/11.
//

#include "AnimationController.h"

namespace EngineTools
{
    AnimationController::AnimationController()
    {
        typeHashCode = typeid(AnimationController).hash_code();
    }

    void AnimationController::AddSequence(ActionSequence *sequence)
    {
        actions.emplace_back(sequence);
    }

    void AnimationController::Update()
    {
        // ���б��е����ж�������
        for (size_t i = 0; i < actions.size(); ++i)
        {
            // ������ִ��һ֡
            bool ret = actions[i]->RunAction();

            // ����������н���������б����Ƴ�
            if (ret)
            {
                actions.erase(actions.begin() + i);
                --i;
            }
        }
    }
    std::string AnimationController::ToString(int depth) const
    {
        std::string ret = "";
        for (int i = 0; i < depth; i++)
        {
            ret += ' ';
        }
        ret += "-Component:AnimationController:\n";

        for (int i = 0; i < depth; i++)
        {
            ret += ' ';
        }
        ret += "@Attached With:";
        if (gameObject)
        {
            ret += gameObject->name;
        }
        ret += '\n';

        return ret;
    }
    
} // EngineTools