//
// Created by tianlan on 2023/5/14.
// �����ڴ��
//

#ifndef SGENGINE_MEMORYPOOL_H
#define SGENGINE_MEMORYPOOL_H

#include <vector>

namespace EngineTools
{
    // T ����Ҫ���������������
    template<typename T>
    class MemoryPool
    {
    private:
        std::vector<T*> alive;  // �����
        std::vector<T*> dead;   // ������
    public:
        // ����������ȡһ������򴴽�����ʼ��һ�����壨�����Ҫ��ʼ����
        // @ para void (*InitFunc)(T* object) = nullptr ����ĳ�ʼ�������������������Ϊ��
        // @ ret  std::pair<T*, bool>                   ��������ָ�룬�Լ���������Ƿ�Ϊ�´��������壬�������Ϊtrue
        std::pair<T*, bool> Create(void (*InitFunc)(T* object) = nullptr);

        // ��һ����������������������
        // @ para T* object ��Ҫ���ٵ�����ָ��
        void Destroy(T* object);
    };


    // ����������ȡһ������򴴽�����ʼ��һ�����壨�����Ҫ��ʼ����
    // @ para void (*InitFunc)(T* object) = nullptr ����ĳ�ʼ�������������������Ϊ��
    // @ ret  std::pair<T*, bool>                   ��������ָ�룬�Լ���������Ƿ�Ϊ�´��������壬�������Ϊtrue
    template<typename T>
    inline std::pair<T*, bool> MemoryPool<T>::Create(void(*InitFunc)(T* object))
    {
        T* res = nullptr;
        bool retBool = false;

        if (dead.empty())
        {
            res = new T;

            if (InitFunc)
            {
                InitFunc(res);
            }
            retBool = true;
        }
        else
        {
            res = dead.back();
            dead.pop_back();
        }

        alive.emplace_back(res);

        return { res, retBool };
    }

    // ��һ����������������������
    // @ para T* object ��Ҫ���ٵ�����ָ��
    template<typename T>
    inline void MemoryPool<T>::Destroy(T* object)
    {
        // ���������Ѱ�Ҳ��Ƴ�
        for (int i = 0; i < alive.size(); i++)
        {
            if (alive[i] == object)
            {
                alive.erase(alive.begin() + i);
                break;
            }
        }

        // ����������
        dead.emplace_back(object);
    }

} // EngineTools

#endif //SGENGINE_MEMORYPOOL_H
