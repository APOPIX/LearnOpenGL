//
//  HelloTriangleExercise3.cpp
//  HelloTriangleEx.3
//
//  课后习题3:创建相同的两个三角形，创建两个着色器程序，第二个程序使用一个不同的片段着色器，
//  输出黄色；再次绘制这两个三角形，让其中一个输出为黄色
//
//  Created by apophis on 2021/5/27.
//

#include <iostream>
#include <thread>
#include <chrono>
#include "glad.h"
#include "glfw3.h"

//顶点数组：z（深度）值为 0.0，代表三个点都在2D空间，(x, y)坐标为标准化设备坐标[-1, 1]
float firstTrangle[] = {//第一个三角形
    -0.5f,  0.0f, 0.0f,
     0.0f,  0.0f, 0.0f,
     0.0f,  0.5f, 0.0f
};
float secondTrangle[] = {//第二个三角形
     0.5f,  0.0f, 0.0f,
     0.0f,  0.0f, 0.0f,
     0.0f, -0.5f, 0.0f
};

//（最简单的）顶点着色器：对输入数据不做任何处理，直接输出
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

// 片段着色器：橘黄色
const char *fragmentShaderSourceOrange = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

// 片段着色器：黄色
const char *fragmentShaderSourceYellow = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\0";

//监听窗口尺寸改变
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(int argc, const char * argv[]) {
    //GLFW创建一个窗口
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //如果是Mac系统，需要调用下面这行代码才能使配置起作用
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //注册回调，监听窗口尺寸变化
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //GLAD加载OpenGL函数指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    unsigned int VBOs[2], VAOs[2];
    //顶点数组对象(Vertex Array Object, VAO)可以像顶点缓冲对象那样被绑定，任何随后的顶点属性调用都会储存在这个VAO中。
    glGenVertexArrays(2, VAOs);
    //顶点缓冲对象(Vertex Buffer Objects, VBO)
    glGenBuffers(2, VBOs);
    //======================= 使用第一个VAO、VBO =======================
    // 1. 首先绑定第一个VAO，激活顶点数组对象，后续的相关操作将作用到该顶点数组对象。
    glBindVertexArray(VAOs[0]);
    // 2. 接着把顶点数组复制到缓冲中供OpenGL使用
    //    绑定第一个VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);//绑定到当前 GL 环境
    //    将顶点数组复制到 VBO 中，GL_STATIC_DRAW ：数据不会或几乎不会改变
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTrangle), firstTrangle, GL_STATIC_DRAW);
    // 3. 设置顶点属性指针
    //    告诉OpenGL该如何解析顶点数据
    //    第一个参数“0“与”layout(location = 0)“中的”0“对应
    //    第二个参数是顶点属性的维度
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //启用顶点属性
    glEnableVertexAttribArray(0);
    //======================= 使用第二个VAO、VBO =======================
    // 1. 接着绑定第二个VAO，激活顶点数组对象，后续的相关操作将作用到该顶点数组对象。
    glBindVertexArray(VAOs[1]);
    // 2. 接着把顶点数组复制到缓冲中供OpenGL使用
    //    绑定第二个VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);//绑定到当前 GL 环境
    //    将顶点数组复制到 VBO 中，GL_STATIC_DRAW ：数据不会或几乎不会改变
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTrangle), secondTrangle, GL_STATIC_DRAW);
    // 3. 设置顶点属性指针
    //    告诉OpenGL该如何解析顶点数据
    //    第一个参数“0“与”layout(location = 0)“中的”0“对应
    //    第二个参数是顶点属性的维度
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //启用顶点属性
    glEnableVertexAttribArray(0);
    //编译顶点着色器
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//加载 shader 代码
    glCompileShader(vertexShader);//编译
    //检查着色器编译是否成功
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //编译片段着色器（橘黄色）
    unsigned int fragmentShaderOrange;
    fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderOrange, 1, &fragmentShaderSourceOrange, NULL);//加载 shader 代码
    glCompileShader(fragmentShaderOrange);//编译
    glGetShaderiv(fragmentShaderOrange, GL_COMPILE_STATUS, &success);
    //检查着色器编译是否成功
     if (!success)
     {
         glGetShaderInfoLog(fragmentShaderOrange, 512, NULL, infoLog);
         std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
     }
    //编译片段着色器（黄色）
    unsigned int fragmentShaderYellow;
    fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderYellow, 1, &fragmentShaderSourceYellow, NULL);//加载 shader 代码
    glCompileShader(fragmentShaderYellow);//编译
    glGetShaderiv(fragmentShaderYellow, GL_COMPILE_STATUS, &success);
    //检查着色器编译是否成功
     if (!success)
     {
         glGetShaderInfoLog(fragmentShaderOrange, 512, NULL, infoLog);
         std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
     }
    //创建橘黄色的程序对象
    unsigned int shaderProgramOrange;
    shaderProgramOrange = glCreateProgram();
    //把之前编译的着色器附加到程序对象上，然后用glLinkProgram链接它们
    glAttachShader(shaderProgramOrange, vertexShader);
    glAttachShader(shaderProgramOrange, fragmentShaderOrange);
    glLinkProgram(shaderProgramOrange);
    //检查是否链接成功
    glGetProgramiv(shaderProgramOrange, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramOrange, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    //创建黄色的程序对象
    unsigned int shaderProgramYellow;
    shaderProgramYellow = glCreateProgram();
    //把之前编译的着色器附加到程序对象上，然后用glLinkProgram链接它们
    glAttachShader(shaderProgramYellow, vertexShader);
    glAttachShader(shaderProgramYellow, fragmentShaderYellow);
    glLinkProgram(shaderProgramYellow);
    //检查是否链接成功
    glGetProgramiv(shaderProgramYellow, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramYellow, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    //在把着色器对象链接到程序对象以后，记得释放着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShaderOrange);
    glDeleteShader(fragmentShaderYellow);
    
    while(!glfwWindowShouldClose(window))
    {//glfwWindowShouldClose函数在我们每次循环的开始前检查一次GLFW是否被要求退出
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//设置以何种颜色来清空屏幕
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除颜色缓冲
        //激活刚刚创建的程序对象
        glUseProgram(shaderProgramOrange);
        //绘制第一个三角形
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glUseProgram(shaderProgramYellow);
        //绘制第二个三角形
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwPollEvents();
        glfwSwapBuffers(window);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    //释放
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(shaderProgramOrange);
    glDeleteProgram(shaderProgramYellow);
    glfwTerminate();
    return 0;
}
