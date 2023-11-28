//
// Created by tianlan on 2023/4/18.
// GameObject类
// 类似Unity中的GameObject
// 是游戏运行过程中的重要组成部分
// 每个物体的具体功能由其添加的组件实现

#ifndef SGENGINE_GAMEOBJECT_H
#define SGENGINE_GAMEOBJECT_H

#include <assert.h>
#include <vector>
#include <algorithm>
#include "Scene.h"
#include "Component.h"
#include "StringTools.h"

namespace EngineTools
{
    class Scene; // 前置声明

    class GameObject
    {
        std::vector<Component *> componentList; // 组件列表
        std::vector<GameObject *> childObjects; // 子物体列表

        // 确认物体信息的修改，一般来说，不需要调用，这一部分工作由Component的OnValidate方法完成
        [[maybe_unused]] void ConfirmAllChange();

    public:
        std::string name; // 物体的名字

        bool isActive = true; // 物体是否启用

        GameObject *parent = nullptr; // 父物体的指针

        Scene* scene = nullptr;

        // Constructors:
        GameObject(); // 无参构造函数，创建一个root物体

        // 有参构造函数，为物体的name字段赋值为_name
        // @para    string  _name   物体名
        explicit GameObject(const std::string &_name);

        ~GameObject(); // 析构函数，并且同时会清除其下所有物体

        // 为当前物体添加组件
        // @para    Component*  component   组件的指针
        void AddComponent(Component *component);

        // 为当前物体添加子物体
        // @para    GameObject* child   子物体的指针
        void AddChild(GameObject *child);

        // 移除当前物体下的某个子物体
        // @para    GameObject* child   子物体的指针
        void RemoveChild(GameObject *child);

        // 根据路径获取子物体指针
        // @para    string  path    子物体路径
        // @ret     GameObject*     子物体指针
        [[nodiscard]] GameObject *GetChild(const std::string &path) const;

        // 直接获得当前物体下的所有子物体
        // @ret     vector<GameObject *>    子物体列表
        [[nodiscard]] std::vector<GameObject *> GetChildren() const;

        void CallStart(); // 调用所有组件的Start方法

        void CallUpdate(); // 调用所有组件的Update方法

        // 获取当前物体的某个组件
        // 因为编译器不支持模板函数声明与定义分离，故写在一起
        // @template    T   组件的类型
        // @ret         T*  组件的指针
        template<typename T>
        [[nodiscard]] T *GetComponent() const
        {
            // 在组件列表里寻找
            for (auto component: componentList)
            {
                // 如果是目标组件，转换成对应类型的指针返回
                if (component->typeHashCode == typeid(T).hash_code())
                {
                    return dynamic_cast<T *>(component);
                }
            }

            // 如果没找到返回nullptr
            return nullptr;
        }

        template<typename T>
        [[nodiscard]] T* RemoveComponent()
        {
            // 在组件列表里寻找
            for (auto it = componentList.begin(); it != componentList.end(); ++it)
            {
                if ((*it)->typeHashCode == typeid(T).hash_code())
                {
                    auto ret = (*it);
                    this->componentList.erase(it);
                    return dynamic_cast<T*>(ret);
                }
            }

            // 如果没找到返回nullptr
            return nullptr;
        }

        virtual std::string ToString(int depth = 0) const;
    };


} // EngineTools

#endif //SGENGINE_GAMEOBJECT_H
