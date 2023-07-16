//
// Created by tianlan on 2023/5/11.
// ��������
//

#ifndef SGENGINE_ACTIONSEQUENCE_H
#define SGENGINE_ACTIONSEQUENCE_H

#include <vector>
#include <initializer_list>
#include "Animation.h"

namespace EngineTools
{
#define ENDLESS_LOOP 0xffffffff // �޾�ѭ��
    class ActionSequence
    {
    private:
        std::vector<std::vector<BaseAnimation*>> actions;   // ���еĶ���������ƽ�ж���
        bool hasEndless = false;    // ��Ǹ��������Ƿ����޾�����
        size_t markEndless = -1;    // ����޾�������λ�ã����û����Ϊ-1
        size_t nowRunning = 0;      // ��ǵ�ǰ�������еĶ�������ֵ
    public:
        ActionSequence()= default;  // Ĭ�Ϲ��캯��

        // ����һ��ƽ�ж������У�Ҫ��������������Щ����ʱ��һ��
        // @ para std::initializer_list<BaseAnimation*> animations ��Ҫ�������еĶ����ǣ���{...}����ʽ����
        // @ ret  std::vector<BaseAnimation*>           �����õ�ƽ������
        static std::vector<BaseAnimation*> MakeParallelAnimations(std::initializer_list<BaseAnimation*> animations);

        // ����������Ӷ�����������ظ�����
        // @ para const std::vector<BaseAnimation*>& animation  ������
        // @ para unsigned int time = 1     ѭ��������Ĭ��Ϊ1
        void AddAnimation(const std::vector<BaseAnimation*>& animation, unsigned int time = 1);

        // �����������������
        // @ para GameObject* object    Ŀ������ָ��
        void BindWith(GameObject* object);

        // ���ж�����ÿ�ε���������һ֡
        // @ ret bool ����������н�������true
        bool RunAction();
    };

} // EngineTools

#endif //SGENGINE_ACTIONSEQUENCE_H
