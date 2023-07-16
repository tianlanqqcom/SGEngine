#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texture;
out vec2 texIn;
out vec3 normalIn;
out vec3 fragPosIn;
uniform mat4 model;             //ģ�;���
uniform mat4 view;              //�۲����
uniform mat4 projection;        //ͶӰ����
void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    texIn = texture;
    fragPosIn = vec3(model * vec4(position, 1.0f));
    normalIn = mat3(transpose(inverse(model))) * normal;
}
