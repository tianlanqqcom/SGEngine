#pragma once
#ifndef MESH_H
#define MESH_H
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
#include"Shader.h"
#include<opengl/glew.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
using namespace std;
struct Vertex
{
    glm::vec3 Position;         //顶点
    glm::vec3 Normal;           //法线
    glm::vec2 TexCoords;        //贴图
};

struct Texture
{
    GLuint id;
    string type;                //贴图类型：漫反射贴图还是镜面贴图（后面还有法线贴图、错位贴图等）
    aiString path;              //贴图路径
};

class Mesh
{
public:

    vector<Vertex> vertices;
    vector<GLuint> indices;             //索引
    vector<Texture> textures;
    Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        this->setupMesh();
    }

    void Draw(Shader shader)
    {
        GLuint diffuseNr = 1;
        GLuint specularNr = 1;
        for (GLuint i = 0; i < this->textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            stringstream ss;
            string name = this->textures[i].type;
            if (name == "texture_diffuse")
                ss << diffuseNr++;
            else if (name == "texture_specular")
                ss << specularNr++;
            name = name + ss.str();
            glUniform1i(glGetUniformLocation(shader.Program, name.c_str()), i);
            //这样的话，着色器中的纹理名就必须有一个对应的规范，例如“texture_diffuse3”代表第三个漫反射贴图
            //方法不唯一，这是最好理解/最简单的一种规范/写法
            glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
        }

        glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 16.0f);     //暂时写死反光度，也可配置
        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);             //EBO绘制
        for (GLuint i = 0; i < this->textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        glBindVertexArray(0);
    }

private:

    GLuint VAO, VBO, EBO;
    void setupMesh()
    {
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        glGenBuffers(1, &this->EBO);

        glBindVertexArray(this->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        //别忘了struct中内存是连续的
        //offsetof()：获取结构体属性的偏移量
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

        glBindVertexArray(0);
    }
};
#endif