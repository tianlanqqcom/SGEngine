#include "PhongShader.h"
#include "EngineTools/ResourceManager.h"


std::pair<double, double> SRender::PhongShader::CalcTexturePos(const EngineTools::Vector3& interpolate,
    const std::pair<double, double>& vt1, 
    const std::pair<double, double>& vt2,
    const std::pair<double, double>& vt3)
{
    std::pair<double, double> res;
    res.first = vt1.first * interpolate.x + vt2.first * interpolate.y + vt3.first * interpolate.z;
    res.second = vt1.second * interpolate.x + vt2.second * interpolate.y + vt3.second * interpolate.z;
    return res;
}

EngineTools::Vector3 SRender::PhongShader::CalcSunLight(EngineTools::Light* sunLight,
    const EngineTools::Vector3& normal, 
    const EngineTools::Vector3& viewDirection, 
    EngineTools::MaterialAsset* material)
{
     // lightDirection = sunLight->lightInfo.sunLight.direction.Normalize();
    EngineTools::Vector3 lightDirection = EngineTools::Vector3(-sunLight->lightInfo.sunLight.direction.x,
        -sunLight->lightInfo.sunLight.direction.y,
        -sunLight->lightInfo.sunLight.direction.z).Normalize();
    float diff = std::max(normal.Dot(lightDirection), 0.0);
    EngineTools::Vector3 diffuse = sunLight->lightInfo.sunLight.diffuse * diff;
    
    EngineTools::Vector3 reflectDir = Reflect(sunLight->lightInfo.sunLight.direction.Normalize(), normal);
    float spec = pow(std::max(viewDirection.Dot(reflectDir), 0.0), material->Ns);
    EngineTools::Vector3 specular = sunLight->lightInfo.sunLight.specular * spec;

    return diffuse + specular;
}

EngineTools::Vector3 SRender::PhongShader::CalcPointLight(EngineTools::Light* pointLight, 
    const EngineTools::Vector3& normal,
    const EngineTools::Vector3& fragPos,
    const EngineTools::Vector3& viewDirection,
    EngineTools::MaterialAsset* material)
{
    EngineTools::Vector3 lightDirection = (pointLight->lightInfo.pointLight.position - fragPos).Normalize();
    float diff = std::max(normal.Dot(lightDirection), 0.0);
    EngineTools::Vector3 diffuse = pointLight->lightInfo.pointLight.diffuse * diff;

    EngineTools::Vector3 reflectDir = Reflect({ -lightDirection.x, -lightDirection .y, -lightDirection .z}, normal);
    float spec = pow(std::max(viewDirection.Dot(reflectDir), 0.0), material->Ns);
    EngineTools::Vector3 specular = pointLight->lightInfo.pointLight.specular * spec;

    float distance = pointLight->lightInfo.pointLight.position.Distance(fragPos);
    float attenuation = 1.0f / (pointLight->lightInfo.pointLight.k0 +
        pointLight->lightInfo.pointLight.k1 * distance +
        pointLight->lightInfo.pointLight.k2 * (distance * distance));

    diffuse = diffuse * attenuation;
    specular = specular * attenuation;
    return diffuse + specular;
}

EngineTools::Vector3 SRender::PhongShader::CalcSpotLight(EngineTools::Light* spotLight,
    const EngineTools::Vector3& normal, 
    const EngineTools::Vector3& fragPos, 
    const EngineTools::Vector3& viewDirection, 
    EngineTools::MaterialAsset * material)
{
    EngineTools::Vector3 lightDirection = (spotLight->lightInfo.spotLight.position - fragPos).Normalize();
    EngineTools::Vector3 negLightDir(-spotLight->lightInfo.spotLight.direction.x,
        -spotLight->lightInfo.spotLight.direction.y, 
        -spotLight->lightInfo.spotLight.direction.z);
    negLightDir = negLightDir.Normalize();

    float theta = lightDirection.Dot(negLightDir.Normalize());
    float lightSoft = Clamp(
        (theta - spotLight->lightInfo.spotLight.outCutOff) / (spotLight->lightInfo.spotLight.cutOff - spotLight->lightInfo.spotLight.outCutOff),
        0.0f,
        1.0f);

    float diff = std::max(normal.Dot(lightDirection), 0.0);
    EngineTools::Vector3 diffuse = spotLight->lightInfo.spotLight.diffuse * diff;

    EngineTools::Vector3 reflectDir = Reflect(negLightDir, normal);
    float spec = powf(std::max(viewDirection.Dot(reflectDir), 0.0), material->Ns);
    EngineTools::Vector3 specular = spotLight->lightInfo.spotLight.specular * spec;

    float distance = spotLight->lightInfo.spotLight.position.Distance(fragPos);
    float attenuation = 1.0f / (spotLight->lightInfo.spotLight.k0 +
        spotLight->lightInfo.spotLight.k1 * distance +
        spotLight->lightInfo.spotLight.k2 * (distance * distance));

    diffuse = diffuse * (lightSoft * attenuation);
    specular = specular * (lightSoft * attenuation);
    return diffuse + specular;
}

double SRender::PhongShader::Clamp(double x, double minVal, double maxVal)
{
    double res = std::max(x, minVal);
    res = std::min(res, maxVal);
    return res;
}

EngineTools::Vector3 SRender::PhongShader::Reflect(const EngineTools::Vector3& I, const EngineTools::Vector3& N)
{
    return I - N * 2.0f * N.Dot(I);
}

RGBAColor SRender::PhongShader::Frag(
    const EngineTools::Vector3& interpolate,
    const EngineTools::Vector3& viewPos,
    const EngineTools::Vector3& fragPos,
    const EngineTools::Vector3& normalIn,
    EngineTools::GlobalSettings* glSet,
    EngineTools::ModelAsset* nowModel, 
    EngineTools::MeshAsset* nowMesh,
    EngineTools::FaceAsset* nowFace)
{
    RGBAColor res;
    EngineTools::Vector3 colorRange(0, 0, 0);

    // 计算纹理坐标
    auto vt = CalcTexturePos(interpolate,
        nowMesh->vertexTexture[nowFace->vertexTextureIndex[0]],
        nowMesh->vertexTexture[nowFace->vertexTextureIndex[1]],
        nowMesh->vertexTexture[nowFace->vertexTextureIndex[2]]);

    EngineTools::MaterialAsset* nowMaterial = nowModel->materials[nowFace->connectedMaterialName];
    auto imgasset = EngineTools::ResourceManager::GetInstance()->FindImageAsset(nowMaterial->map_Kd);
    RGBAColor baseColor;

    // 如果纹理坐标合法，取纹理的对应点为基颜色
    if (vt.first >= 0.0f && vt.first <= 1.0f && vt.second >= 0.0f && vt.second <= 1.0f)
    {
        baseColor = imgasset->ImgData[vt.second * (imgasset->ImageHeight - 1)][static_cast<int>(vt.first * (imgasset->ImageWidth - 1))];
    }
    else
    {
        baseColor = RGBAColor(107, 107, 107, 255);
    }

    // 获取环境光
    EngineTools::Vector3 ambientColor;
    auto& glAmbients = glSet->orderedLights[EngineTools::LightType::Ambient];

    // 如果场景里存在环境光信息，取对应值作为环境光
    // 如果有多个环境光，只取列表第一个
    if (glAmbients.size() > 0)
    {
        ambientColor = glAmbients[0]->lightInfo.ambient;
    }
    // 如果没有，取默认值（0.1，0.1，0.1）
    else
    {
        ambientColor = EngineTools::Vector3(0.1, 0.1, 0.1);
    }

    // 计算视角方向并标准化
    EngineTools::Vector3 viewDirection = (viewPos - fragPos).Normalize();

    // 计算太阳光贡献，如果有多个只取第一个
    auto& sunLightList = glSet->orderedLights[EngineTools::LightType::SunLight];
    if (sunLightList.size() > 0)
    {
        colorRange = CalcSunLight(sunLightList[0], normalIn, viewDirection, nowMaterial);
    }

    // 计算点光源贡献
    auto& pointLightList = glSet->orderedLights[EngineTools::LightType::PointLight];
    for (auto& pointLight : pointLightList)
    {
        colorRange = colorRange + CalcPointLight(pointLight, normalIn, fragPos, viewDirection, nowMaterial);
    }

    // 计算聚光灯贡献
    auto& spotLightList = glSet->orderedLights[EngineTools::LightType::SpotLight];
    for (auto& spotLight : spotLightList)
    {
        colorRange = colorRange + CalcSpotLight(spotLight, normalIn, fragPos, viewDirection, nowMaterial);
    }

    // 限定颜色范围为[环境光-1.0]
    if (colorRange.x > 1.0)
    {
        colorRange.x = 1.0;
    }
    else if (colorRange.x < ambientColor.x)
    {
        colorRange.x = ambientColor.x;
    }

    if (colorRange.y > 1.0)
    {
        colorRange.y = 1.0;
    }
    else if (colorRange.y < ambientColor.y)
    {
        colorRange.y = ambientColor.y;
    }

    if (colorRange.z > 1.0)
    {
        colorRange.z = 1.0;
    }
    else if (colorRange.z < ambientColor.z)
    {
        colorRange.z = ambientColor.z;
    }

    // 计算结果颜色
    res.red = std::max(colorRange.x, 0.0) * baseColor.red;
    res.green = std::max(colorRange.y, 0.0)* baseColor.green;
    res.blue = std::max(colorRange.z, 0.0) * baseColor.blue;
    res.alpha = 255;
    
    return res;
}
