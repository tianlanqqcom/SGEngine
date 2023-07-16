//
// Created by tianlan on 2023/5/3.
// 光照信息
// 
// Edited when Engine Version Updating from 1.0->2.0.
// 
// 现在不论什么类型的光照，都看作为一个Light组件
// Light组件需要注册到全局信息才能生效
// 
// class Light;
//

#ifndef SGENGINE_LIGHT_H
#define SGENGINE_LIGHT_H

#include "../Component.h"
#include "../Vector3.h"


namespace EngineTools
{
    // 光照类型
    enum class LightType
    {
        SunLight,
        PointLight,
        SpotLight,
        Ambient
    };

    struct SunLight             //平行光
    {
        Vector3 direction;
        Vector3 diffuse;
        Vector3 specular;
    };

    struct PointLight           //点光源
    {
        Vector3 position;
        Vector3 diffuse;
        Vector3 specular;
        float k0, k1, k2;
    };
    struct SpotLight            //聚光灯
    {
        Vector3 position;
        Vector3 direction;
        Vector3 diffuse;
        Vector3 specular;
        float k0, k1, k2;
        float cutOff, outCutOff;
    };

    class Light : public Component
    {
        static int lightCount;      // 已创建的光照个数
    public:
        int lightID;                // 光照ID
        LightType lightType;        // 光照类型
        union LightInfo
        {
            SunLight sunLight;      // 太阳光
            PointLight pointLight;  // 点光源
            SpotLight spotLight;    // 聚光灯
            Vector3 ambient;        // 环境光
        } lightInfo;                // 光照信息

        Light();

        void Start();

        std::string ToString(int depth = 0) const override;

        // 注册到全局信息
        void RegisterToGlobalSettings();
    };

} // EngineTools

#endif //SGENGINE_LIGHT_H
