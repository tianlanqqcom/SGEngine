#version 330 core
struct Material
{
    sampler2D texture_diffuse1;      //贴图
    sampler2D texture_specular1;     //镜面贴图
    sampler2D emission;     //放射贴图
    float shininess;        //反光度
};
struct SunLight             //平行光
{
    vec3 direction;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight           //点光源
{
    vec3 position;
    vec3 diffuse;
    vec3 specular;
    float k0, k1, k2;
};
struct SpotLight            //聚光灯
{
    vec3 position;
    vec3 direction;
    vec3 diffuse;
    vec3 specular;
    float k0, k1, k2;
    float cutOff, outCutOff;
};
uniform vec3 ambient;
uniform Material material;
uniform SunLight sunLight;
uniform PointLight pointLights[3];
uniform SpotLight spotLight;
vec3 CalcSunLight(SunLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
out vec4 color;
uniform vec3 viewPos;
in vec2 texIn;
in vec3 fragPosIn;
in vec3 normalIn;
void main()
{
    //环境光
    vec3 ambient = ambient * vec3(texture(material.texture_diffuse1, texIn));
    vec3 viewDir = normalize(viewPos - fragPosIn);
    vec3 normal = normalize(normalIn);
    vec3 result = CalcSunLight(sunLight, normal, viewDir);
    for (int i = 0; i <= 2; i++)
        result = result + CalcPointLight(pointLights[i], normal, fragPosIn, viewDir);
    result = result + CalcSpotLight(spotLight, normal, fragPosIn, viewDir);
    color = vec4(result, 1.0f);
}
 
vec3 CalcSunLight(SunLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.texture_diffuse1, texIn)));
 
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * vec3(texture(material.texture_specular1, texIn)));
 
    return diffuse + specular;
}
 
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.texture_diffuse1, texIn)));
 
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * vec3(texture(material.texture_specular1, texIn)));
 
    float dis = length(light.position - fragPos);
    float attenuation = 1.0f / (light.k0 + light.k1 * dis + light.k2 * (dis * dis));
 
    diffuse *= attenuation;
    specular *= attenuation;
    return diffuse + specular;
}
 
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float lightSoft = clamp((theta - light.outCutOff) / (light.cutOff - light.outCutOff), 0.0f, 1.0f);
 
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.texture_diffuse1, texIn)));
 
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * vec3(texture(material.texture_specular1, texIn)));
 
    float dis = length(light.position - fragPos);
    float attenuation = 1.0f / (light.k0 + light.k1 * dis + light.k2 * (dis * dis));
 
    diffuse *= attenuation * lightSoft;
    specular *= attenuation * lightSoft;
    return diffuse + specular;
}
