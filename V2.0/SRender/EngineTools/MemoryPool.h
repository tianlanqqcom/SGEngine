//
// Created by tianlan on 2023/5/14.
// 简易内存池
//

#ifndef SGENGINE_MEMORYPOOL_H
#define SGENGINE_MEMORYPOOL_H

#include <vector>

namespace EngineTools
{
    // T 是需要创建的物体的类型
    template<typename T>
    class MemoryPool
    {
    private:
        std::vector<T*> alive;  // 生存池
        std::vector<T*> dead;   // 死亡池
    public:
        // 从生存池里获取一个物体或创建并初始化一个物体（如果需要初始化）
        // @ para void (*InitFunc)(T* object) = nullptr 物体的初始化方法，如果不传入则为空
        // @ ret  std::pair<T*, bool>                   返回物体指针，以及这个物体是否为新创建的物体，如果是则为true
        std::pair<T*, bool> Create(void (*InitFunc)(T* object) = nullptr);

        // 将一个物体从生存池移入死亡池
        // @ para T* object 将要销毁的物体指针
        void Destroy(T* object);
    };


    // 从生存池里获取一个物体或创建并初始化一个物体（如果需要初始化）
    // @ para void (*InitFunc)(T* object) = nullptr 物体的初始化方法，如果不传入则为空
    // @ ret  std::pair<T*, bool>                   返回物体指针，以及这个物体是否为新创建的物体，如果是则为true
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

    // 将一个物体从生存池移入死亡池
    // @ para T* object 将要销毁的物体指针
    template<typename T>
    inline void MemoryPool<T>::Destroy(T* object)
    {
        // 在生存池中寻找并移除
        for (int i = 0; i < alive.size(); i++)
        {
            if (alive[i] == object)
            {
                alive.erase(alive.begin() + i);
                break;
            }
        }

        // 加入死亡池
        dead.emplace_back(object);
    }

} // EngineTools

#endif //SGENGINE_MEMORYPOOL_H
