//
// Created by tianlan on 2023/4/18.
//

#include "GameObject.h"
#include "StringTools.h"

namespace EngineTools
{
    unsigned int GameObject::ObjectID = 0; // 初始化静态变量

    GameObject::GameObject()
    {
        name = "root";
        UID = ObjectID++;
    }

    GameObject::GameObject(const std::string &_name)
    {
        name = _name;
        UID = ObjectID++;
    }

    GameObject::~GameObject()
    {
        // 先清除所有子物体，这里涉及到递归
        for (auto child: childObjects)
        {
            delete child;
        }

        // 清除所有组件
        for (auto& component: componentList)
        {
            delete component.second;
        }
    }

    void GameObject::AddComponent(Component *component)
    {
        // 检查指针是否为空
        if (component)
        {
            // 检查是否已经含有该类型的组件
            //for (auto& confirmed: componentList)
            //{
            //    // 如果重复，抛出异常
            //    if (confirmed->typeHashCode == component->typeHashCode)
            //    {
            //        throw std::exception(); // 后续会换成引擎专有异常
            //    }
            //}
            auto it = componentList.find(component->typeHashCode);
            if (it != componentList.end())
            {
                throw std::exception();
            }

            // 添加组件
            componentList[component->typeHashCode] = component; // 添加到列表
            component->gameObject = this;
            component->OnValidate(); // 确认添加
        }
        else
        {
            throw std::exception(); // 如果参数是nullptr, 抛出异常, 后续会换成引擎专有异常（或许我应该用assert?）
        }
    }

    void GameObject::AddChild(GameObject *child)
    {
        // 检查指针
        if (child)
        {
            // 如果物体已经有父物体，抛出异常
            if (child->parent)
            {
                throw std::exception(); // 后续会换成引擎专有异常
            }

            // 添加物体
            child->parent = this;
            child->scene = this->scene;
            childObjects.emplace_back(child);
        }
        else
        {
            throw std::exception(); // 后续会换成引擎专有异常
        }
    }

    void GameObject::RemoveChild(GameObject* child)
    {
        assert(child);

        if (child->parent == this)
        {
            child->scene = nullptr;
            child->parent = nullptr;

            for (auto it = childObjects.begin(); it != childObjects.end(); ++it)
            {
                if (*it == child)
                {
                    this->childObjects.erase(it);
                    break;
                }
            }
        }
        else
        {
            throw std::exception();
        }
    }

    GameObject *GameObject::GetChild(const std::string &path) const
    {
        // 先处理路径
        std::vector<std::string> names; // 路径上所有物体的名字
        Stringsplit(path, '/', names); // 给names赋值

        // 异常情况判定
        // 如果路径不可达，返回空指针
        if (names.empty() || names[0] != this->name)
        {
            return nullptr;
        }
            // 如果要找的就是当前物体，返回自己
        else if (names.size() == 1 && names[0] == this->name)
        {
            return const_cast<GameObject *>(this);
        }
        else
        {
            GameObject *res; // 将要返回的结果
            auto *nowObject = const_cast<GameObject *>(this); // 最开始指向自己
            int depth = 1; // 当前搜索深度
            auto maxIndex = names.size(); // 最大的索引值，不能大于等于它，会越界
            bool selectNewObject; // 判定某次循环是否选定了新物体

            // 开始寻找
            while (true)
            {
                selectNewObject = false;

                // 在当前物体的子物体列表里寻找名字是当前搜索深度的物体
                for (auto& child: nowObject->childObjects)
                {
                    // 如果找到
                    if (child->name == names[depth])
                    {
                        // 如果达到路径里的最后一个，返回这个物体
                        ++depth;
                        if (depth == maxIndex)
                        {
                            res = child;
                            return res;
                        }

                        // 更新当前物体
                        nowObject = child;

                        // 更新选定标识
                        selectNewObject = true;

                        // 开始下一次寻找
                        break;
                    }
                }

                // 如果某轮寻找没有找到物体
                if (!selectNewObject)
                {
                    throw std::exception(); // 抛出异常，后续会换成引擎专有异常
                }
            }
        }
    }

    std::vector<GameObject *> GameObject::GetChildren() const
    {
        // 直接返回列表
        return childObjects;
    }

    [[maybe_unused]] void GameObject::ConfirmAllChange()
    {
        // 调用所有组件的OnValidate方法
        for (auto& component : componentList)
        {
            component.second->OnValidate();
        }
    }

    void GameObject::CallUpdate()
    {
        // 调用所有已启用的组件的Update方法
        for (auto& component: componentList)
        {
            if (component.second->isActive)
            {
                component.second->Update();
            }
        }

        // 让所有已经启用的子物体也递归调用
        for (auto& child: childObjects)
        {
            if (child->isActive)
            {
                child->CallUpdate();
            }
        }
    }

    std::string GameObject::ToString(int depth) const
    {
        std::string ret = "";

        for (int i = 0; i < depth; i++)
        {
            ret += ' ';
        }

        ret += '-';
        ret += this->name;
        ret += '\n';

        for (auto& component : componentList)
        {
            ret += component.second->ToString(depth + 1);
        }

        for (auto& obj : childObjects)
        {
            ret += obj->ToString(depth + 1);
        }

        return ret;
    }

    void GameObject::CallStart()
    {
        transform = GetComponent<Transform>();
        // 调用所有已启用的组件的Start方法
        for (auto& component: componentList)
        {
            if (component.second->isActive)
            {
                component.second->Start();
            }
        }

        // 让所有已经启用的子物体也递归调用
        for (auto& child: childObjects)
        {
            if (child->isActive)
            {
                child->CallStart();
            }
        }
    }



//    template<typename T>
//    T *GameObject::GetComponent()
//    {
//        for (auto component: componentList)
//        {
//            if (component->typeHashCode == typeid(T).hash_code())
//            {
//                return static_cast<T *>(component);
//            }
//        }
//        return nullptr;
//    }
} // EngineTools