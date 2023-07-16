//
// Created by tianlan on 2023/4/23.
// 这是一个场景类，
// 创建时自动含有一个带有Transform组件的root物体，
// 可以被继承从而创建你自己的场景

#ifndef SGENGINE_SCENE_H
#define SGENGINE_SCENE_H

#include <vector>
#include "GameObject.h"
#include "Component.h"

namespace EngineTools
{

    class Scene
    {
    public:
        GameObject *root; // 根结点

        std::vector<Component*> sharedComponents; // 该场景里的共享组件

        // Constructors:
        Scene(); // 无参构造函数，自动创建一个root物体并添加Transform组件
        ~Scene(); // 析构函数

        // Methods:

        // 根据传入的路径获取子物体的指针
        // @para    string  path 子物体的路径
        // @ret     GameObject*  子物体的指针
        [[nodiscard]] GameObject *GetChild(const std::string &path) const;

        // 根据路径为场景添加一个子物体
        // @para    string  path 子物体的路径
        void AddChild(const std::string &path);

        // 在场景的共享组件列表里添加组件
        // @para    Component*  component   组件的指针
        void AddSharedComponent(Component* component);

        // 在场景的共享组件列表里移除组件
        // @para    Component*  component   组件的指针
        void RemoveSharedComponent(Component* component);

        // 在场景的共享组件列表里移除组件
        // @template    T   组件的类型
        template<typename T>
        void RemoveSharedComponent()
        {
            for (auto it = sharedComponents.begin(); it != sharedComponents.end(); it++)
            {
                if ((*it)->typeHashCode == typeid(T).hash_code())
                {
                    sharedComponents.erase(it);
                    return;
                }
            }
            throw std::exception(); // 如果列表里没有该组件，抛出异常
        }

        // 调用该场景中所有物体的Start方法
        void CallStart();

        // 调用该场景中所有物体的Update方法
        void CallUpdate();

        // 获取当前物体的某个组件
        // 因为编译器不支持模板函数声明与定义分离，故写在一起
        // @template    T   组件的类型
        // @ret         T*  组件的指针
        template<typename T>
        [[nodiscard]] T* GetSharedComponent() const
        {
            // 在组件列表里寻找
            for (auto component : sharedComponents)
            {
                // 如果是目标组件，转换成对应类型的指针返回
                if (component->typeHashCode == typeid(T).hash_code())
                {
                    return dynamic_cast<T*>(component);
                }
            }

            // 如果没找到返回nullptr
            return nullptr;
        }

        virtual std::string ToString(int depth = 0) const;
    };

} // EngineTools

#endif //SGENGINE_SCENE_H
