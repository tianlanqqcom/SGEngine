//
// Created by tianlan on 2023/4/18.
// Component类
// 类似Unity中的Component
// 物体的具体功能在这里实现

#ifndef SGENGINE_COMPONENT_H
#define SGENGINE_COMPONENT_H

#include <iostream>


namespace EngineTools
{
    class GameObject;

    class Transform;

    class Component
    {
    public:
        bool isActive = true; // 是否启用

        GameObject *gameObject = nullptr; // 父物体的指针

        size_t typeHashCode; // 类型判定依据，用于GetComponent方法，在被继承时应当在构造函数里更改初始设定值

        // Constructors:
        Component(); // 无参构造函数，必须在这里为typeHashCode赋值

        virtual ~Component() = default; // 虚析构函数，可以覆盖

        // Methods:
        void Enable(); // 启用组件

        void Disable(); // 禁用组件

        virtual void OnValidate() // 用于确认信息更改
        {};

        virtual void Awake() // 场景初调用，只会被调用一次
        {};

        virtual void OnEnable() // 被启用时调用
        {};

        virtual void OnDisable() // 被禁用时调用
        {};

        virtual void Start() // 场景开始运行的时候调用
        {}

        virtual void Update() // 每帧被调用
        {};

        virtual void OnRemove() // 组件被移除时调用
        {};

        virtual std::string ToString(int depth = 0) const;
    };
}

#endif //SGENGINE_COMPONENT_H
