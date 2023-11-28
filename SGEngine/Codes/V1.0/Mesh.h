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
    glm::vec3 Position;         //����
    glm::vec3 Normal;           //����
    glm::vec2 TexCoords;        //��ͼ
};

struct Texture
{
    GLuint id;
    string type;                //��ͼ���ͣ���������ͼ���Ǿ�����ͼ�����滹�з�����ͼ����λ��ͼ�ȣ�
    aiString path;              //��ͼ·��
};

class Mesh
{
public:

    vector<Vertex> vertices;
    vector<GLuint> indices;             //����
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
            //�����Ļ�����ɫ���е��������ͱ�����һ����Ӧ�Ĺ淶�����硰texture_diffuse3�������������������ͼ
            //������Ψһ������������/��򵥵�һ�ֹ淶/д��
            glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
        }

        glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 16.0f);     //��ʱд������ȣ�Ҳ������
        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);             //EBO����
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
        //������struct���ڴ���������
        //offsetof()����ȡ�ṹ�����Ե�ƫ����
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

        glBindVertexArray(0);
    }
};
#endif