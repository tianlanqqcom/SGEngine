#ifndef CAMERA_H
#define CAMERA_H
#include<iostream>
#include<vector>
#include<opengl/glew.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

const GLfloat YAW = 0.0f;           //y轴，偏航
const GLfloat PITCH = 0.0f;         //x轴，俯仰
const GLfloat ZOOM = 45.0f;         //视角，用于缩放
const GLfloat MAXZOOM = 100.0f;
const GLfloat MINZOOM = 1.0f;
const GLfloat SPEED = 1.0f;         //速度，用于移动
const GLfloat SENSITIVTY = 2.0f;   //鼠标灵敏度

class Camera
{
public:
    glm::vec3 Position;
    glm::vec3 Front, Up, Right;
    glm::vec3 WorldUp;
    GLfloat Yaw, Pitch;
    GLfloat MovementSpeed;
    GLfloat MouseSensitivity;
    GLfloat Zoom;
    int width;
    int height;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        GLfloat yaw = YAW, GLfloat pitch = PITCH,
        int width = 800, int height = 600) :
        Front(glm::vec3(0.0f, 0.0f, -1.0f)),
        MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM),
        width(width), height(height)
    {
        this->Position = position;
        this->WorldUp = up;
        this->Yaw = yaw;
        this->Pitch = pitch;
        this->updateCameraVectors();
    }

    Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) :
        Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
    {
        this->Position = glm::vec3(posX, posY, posZ);
        this->WorldUp = glm::vec3(upX, upY, upZ);
        this->Yaw = yaw;
        this->Pitch = pitch;
        this->updateCameraVectors();
    }

    void ChangeScreenSize(float width, float height)
    {
        this->width = width;
        this->height = height;
    }

    //获取对应的LookAt矩阵
    glm::mat4 GetViewMatrix()
    {
        //printf("%.2f, %.2f, %.2f\n", this->Position.x, this->Position.y, this->Position.z);
        return glm::lookAt(this->Position, this->Position + this->Front, this->WorldUp);
    }

    void ProcessKeyboard(int direction, GLfloat deltaTime)
    {
        // std::cout << "Camera move" << std::endl;
        GLfloat velocity = this->MovementSpeed * deltaTime;
        if (direction == FORWARD)
            this->Position += this->Front * velocity;
        if (direction == BACKWARD)
            this->Position -= this->Front * velocity;
        if (direction == LEFT)
            this->Position -= this->Right * velocity;
        if (direction == RIGHT)
            this->Position += this->Right * velocity;
        if (direction == UP)
            this->Position += this->Up * velocity;
        if (direction == DOWN)
            this->Position -= this->Up * velocity;
    }

    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= this->MouseSensitivity;
        yoffset *= this->MouseSensitivity;

        this->Yaw += xoffset;
        this->Pitch += yoffset;
        if (constrainPitch)
        {
            if (this->Pitch > 89.0f)
                this->Pitch = 89.0f;
            if (this->Pitch < -89.0f)
                this->Pitch = -89.0f;
        }
        this->updateCameraVectors();
    }

    void ProcessMouseScroll(GLfloat yoffset)
    {
        if (this->Zoom >= MINZOOM && this->Zoom <= MAXZOOM)
            this->Zoom -= yoffset;
        if (this->Zoom <= MINZOOM)
            this->Zoom = MINZOOM;
        if (this->Zoom >= MAXZOOM)
            this->Zoom = MAXZOOM;
    }

private:
    void updateCameraVectors()
    {
        glm::vec3 front;
        front.x = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
        front.y = sin(glm::radians(this->Pitch));
        front.z = -cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
        this->Front = glm::normalize(front);
        this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
        this->Up = glm::normalize(glm::cross(this->Right, this->Front));
    }
};
#endif