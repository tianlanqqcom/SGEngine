//
// Created by tianlan on 2023/5/3.
// 渲染组件，基于OpenGl和glfw实现
//

#ifndef SGENGINE_RENDER_H
#define SGENGINE_RENDER_H

#include "../../Camera.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include "../GameObject.h"
#include "../Component.h"
#include "../Components/Transform.h"
#include "Light.h"
#include<opengl/glew.h>
#define GLEW_STATIC
#include<GLFW/glfw3.h>
#include "../../Model.h"


namespace EngineTools
{

    class Render : public Component
    {
        Camera* camera = nullptr;   // 相机
        Model* pModel = nullptr;    // 模型
        Shader* shaderObj = new Shader("ObjectVShader.txt", "ObjectFShader.txt");   // 默认GLSL Shader
        bool flag = false;

        void SetLight();    // 设置光照数据
    public:
        Render();
        void Update() override;

        // 绑定模型
        // @ para std::string modelPath 模型文件路径
        void SetModel(std::string modelPath);

        // 绑定相机
        // @ para Camera* camera    相机指针
        void SetCamera(Camera* camera);

        // 绑定着色器
        // @ para Shader* shader    Shader指针
        void SetShader(Shader* shader);

        // 输出组件信息
        std::string ToString(int depth = 0) const override;
    };

} // EngineTools

#endif //SGENGINE_RENDER_H
