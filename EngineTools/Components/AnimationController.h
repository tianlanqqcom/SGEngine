//
// Created by tianlan on 2023/5/11.
// ���������������Ƴ����ж����Ľ���
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
        std::vector<ActionSequence*> actions;   // ���������б�
    public:
        AnimationController();

        // ����������������
        // @ para ActionSequence* sequence  ��Ҫ��ӵ����е�ָ��
        void AddSequence(ActionSequence* sequence);

        void Update() override;

        std::string ToString(int depth = 0) const override;
    };

} // EngineTools

#endif //SGENGINE_ANIMATIONCONTROLLER_H
