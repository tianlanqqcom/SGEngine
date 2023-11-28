//
// Created by tianlan on 2023/4/18.
// Transform组件，与Unity中的Transform类似
//

#ifndef SGENGINE_TRANSFORM_H
#define SGENGINE_TRANSFORM_H

#include "../Component.h"
#include "../Vector3.h"

namespace EngineTools
{
    class Transform : public Component
    {
    public:
        Vector3 localPosition{};    // 以父物体位置为原点的相对位置
        Vector3 worldPosition{};    // 世界位置
        // 旋转角度，注意：当父物体旋转时不影响子物体的旋转，如果需要跟随旋转，请在子物体使用RotationWithRoot方法
        Vector3 rotation{};         
        Vector3 scale{1, 1, 1};     // 缩放，同上，父物体缩放不影响子物体

        Transform();

        void OnValidate() override; // 确认世界坐标是否更改
        void Awake() override{};    // 未使用
        void OnEnable() override{}; // 未使用
        void Start() override;      // 确认世界坐标是否更改
        void Update() override;     // 确认世界坐标是否更改
        void OnDisable() override{};// 未使用
        void OnRemove() override{}; // 未使用

        // 对物体进行平移变换
        // @ para const Vector3& vec3   移动的方向及距离
        void Translate(const Vector3& vec3);    

        // 旋转物体
        // @ para const Vector3& vec3 旋转角度，以度为单位
        void Rotation(const Vector3& vec3);

        // 跟随父物体旋转
        // @ para const Vector3& vec3 旋转角度，以度为单位
        // @ para bool changeSelfRotation = true    是否改变当前物体旋转角度，如果false，则仅更改位置而不改变旋转角度
        void RotationWithRoot(const Vector3& vec3, bool changeSelfRotation = true);

        // ToString方法，显示数据成员
        std::string ToString(int depth = 0) const override;
    };

} // EngineTools

#endif //SGENGINE_TRANSFORM_H
