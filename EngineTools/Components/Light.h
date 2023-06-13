//
// Created by tianlan on 2023/5/3.
// 光照信息
// 暂时设置为固定光源数量，与默认Shader对应
//

#ifndef SGENGINE_LIGHT_H
#define SGENGINE_LIGHT_H

#include "../Component.h"
#include "../Vector3.h"

namespace EngineTools
{
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
    public:
        SunLight sunLight;              // 太阳光
        PointLight pointLight[3] = {};  // 点光源
        SpotLight spotLight;            // 聚光灯
        Vector3 ambient;                // 环境光

        Light();

        std::string ToString(int depth = 0) const override;
    };

} // EngineTools

#endif //SGENGINE_LIGHT_H
