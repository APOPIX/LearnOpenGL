#version 330 core
layout (location = 0) in vec3 aPos;   // 位置变量的属性位置值为 0

out vec4 position; // 向片段着色器输出当前位置

void main()
{
    gl_Position = vec4(aPos, 1.0);
    position = gl_Position; // 向片段着色器输出当前的位置
}
