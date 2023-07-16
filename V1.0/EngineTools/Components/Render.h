//
// Created by tianlan on 2023/5/3.
// ��Ⱦ���������OpenGl��glfwʵ��
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
        Camera* camera = nullptr;   // ���
        Model* pModel = nullptr;    // ģ��
        Shader* shaderObj = new Shader("ObjectVShader.txt", "ObjectFShader.txt");   // Ĭ��GLSL Shader
        bool flag = false;

        void SetLight();    // ���ù�������
    public:
        Render();
        void Update() override;

        // ��ģ��
        // @ para std::string modelPath ģ���ļ�·��
        void SetModel(std::string modelPath);

        // �����
        // @ para Camera* camera    ���ָ��
        void SetCamera(Camera* camera);

        // ����ɫ��
        // @ para Shader* shader    Shaderָ��
        void SetShader(Shader* shader);

        // ��������Ϣ
        std::string ToString(int depth = 0) const override;
    };

} // EngineTools

#endif //SGENGINE_RENDER_H
