//
// Created by tianlan on 2023/4/23.
//

#include "Scene.h"
#include "Components/Transform.h"

namespace EngineTools
{
    Scene::Scene()
    {
        // 创建root物体
        root = new GameObject("root");

        root->scene = this; // 令root的场景指针为当前场景

        // 为root添加Transform组件
        root->AddComponent(new Transform());
    }

    Scene::~Scene()
    {
        // 释放root物体,
        // 这将调用GameObject类的析构函数，进而清除所有在该物体下的子物体
        delete root;
    }

    GameObject *Scene::GetChild(const std::string &path) const
    {
        // 调用GameObject的GetChild方法获取物体指针
        return root->GetChild(path);
    }

    void Scene::AddChild(const std::string &path)
    {
        // 因为GameObject类的AddChild方法只接受物体指针
        // 所以要在这里先找到路径里的倒数第二项（即父物体），并且创建一个新物体

        // 先处理路径
        std::vector<std::string> names; // 路径上所有物体的名字
        Stringsplit(path, '/', names); // 给names赋值

        // 如果names的长度小于等于1，说明这个路径有问题，比如是空路径或者希望与root并列
        if (names.size() <= 1)
        {
            throw std::exception(); // 抛出异常，后续将替换成引擎专有异常
            // @Q: 为什么不直接return而是抛出异常？
            // @A: 直接return会让程序员误以为这个物体添加成功了，而抛出异常则是为了告诉他们这里有问题
        }

        // 这一步把刚才拆开的路径再放回去，但是只到父物体的路径
        std::string childParentName;
        for (int i = 0; i < names.size() - 1; ++i)
        {
            if (i)
            {
                childParentName += "/";
            }
            childParentName += names[i];
        }

        // 找到父物体
        auto childParent = root->GetChild(childParentName);

        // 为父物体添加子物体，名字为names中的最后一个
        childParent->AddChild(new GameObject(names[names.size() - 1]));
    }

    void Scene::CallUpdate()
    {
        // 调用所有共享组件的Update方法
        for (auto shared : sharedComponents)
        {
            shared->Update();
        }

        // 调用GameObject类的CallUpdate方法
        root->CallUpdate();
    }

    std::string Scene::ToString(int depth) const
    {
        std::string ret = "";
        for (int i = 0; i < depth; i++)
        {
            ret += ' ';
        }

        ret += "-Scene\n";

        for (auto& shared : sharedComponents)
        {
            ret += shared->ToString(depth + 1);
        }

        ret += root->ToString(depth + 1);
        return ret;
    }

    void Scene::AddSharedComponent(Component* component)
    {
        for (auto confirmed : sharedComponents)
        {
            if (confirmed->typeHashCode == component->typeHashCode)
            {
                throw std::exception(); // 如果已有相同组件，抛出异常
            }
        }

        sharedComponents.emplace_back(component);
    }

    void Scene::RemoveSharedComponent(Component* component)
    {
        for (auto it = sharedComponents.begin(); it != sharedComponents.end(); it++)
        {
            if (*it == component)
            {
                sharedComponents.erase(it);
                return;
            }
        }
        throw std::exception(); // 如果列表里没有该组件，抛出异常
    }

    void Scene::CallStart()
    {
        // 调用所有共享组件的Start方法
        for (auto shared : sharedComponents)
        {
            shared->Start();
        }

        // 调用GameObject类的CallStart方法
        root->CallStart();
    }
} // EngineTools