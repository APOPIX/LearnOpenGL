//
//  HelloTriangleExercise1.cpp
//  HelloTriangleExercise1
//
//  课后习题1:使用glDrawArrays绘制两个彼此相连的三角形
//
//  Created by apophis on 2021/5/12.
//

#include <iostream>
#include <thread>
#include <chrono>
#include "glad.h"
#include "glfw3.h"

//顶点数组：z（深度）值为 0.0，代表三个点都在2D空间，(x, y)坐标为标准化设备坐标[-1, 1]
float vertices[] = {
    //右上方的三角形
    0.5f, 0.5f, 0.0f,   // 右上角
    0.5f, -0.5f, 0.0f,  // 右下角
    -0.5f, 0.5f, 0.0f,  // 左上角
    //左下方的三角形
    0.5f, -0.5f, 0.0f,  // 右下角
    -0.5f, -0.5f, 0.0f, // 左下角
    -0.5f, 0.5f, 0.0f   // 左上角
};

//（最简单的）顶点着色器：对输入数据不做任何处理，直接输出
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

//（最简单的）片段着色器：只输出某一种颜色（橘黄色）
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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

    unsigned int VBO, VAO;
    //顶点数组对象(Vertex Array Object, VAO)可以像顶点缓冲对象那样被绑定，任何随后的顶点属性调用都会储存在这个VAO中。
    glGenVertexArrays(1, &VAO);
    //顶点缓冲对象(Vertex Buffer Objects, VBO)
    glGenBuffers(1, &VBO);
    // 1. 首先绑定VAO，激活顶点数组对象，后续的相关操作将作用到该顶点数组对象。
    glBindVertexArray(VAO);
    // 2. 接着把顶点数组复制到缓冲中供OpenGL使用
    //    绑定VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);//绑定到当前 GL 环境
    //    将顶点数组复制到 VBO 中，GL_STATIC_DRAW ：数据不会或几乎不会改变
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
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
    //编译片段着色器
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);//加载 shader 代码
    glCompileShader(fragmentShader);//编译
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    //检查着色器编译是否成功
     if (!success)
     {
         glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
         std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
     }
    //如果要使用刚才编译的着色器,必须把它们链接(Link)为一个着色器程序对象
    //创建一个程序对象
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    //把之前编译的着色器附加到程序对象上，然后用glLinkProgram链接它们
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //检查是否链接成功
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    //在把着色器对象链接到程序对象以后，记得释放着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    int count = 0, polygonMode = GL_FILL;//控制以线框还是填充模式绘制三角形
    while(!glfwWindowShouldClose(window))
    {//glfwWindowShouldClose函数在我们每次循环的开始前检查一次GLFW是否被要求退出
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//设置以何种颜色来清空屏幕
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除颜色缓冲
        //激活刚刚创建的程序对象
        glUseProgram(shaderProgram);
        //绑定VAO
        glBindVertexArray(VAO);
        //绘制三角形
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glfwPollEvents();
        glfwSwapBuffers(window);
        //这段逻辑仅用于控制以线框还是填充模式绘制三角形，展示效果用
        if (count++ > 50) {
            //距离上一次切换线框/色块模式循环超过了三秒
            if (polygonMode == GL_FILL) {
                polygonMode = GL_LINE;
            } else {
                polygonMode = GL_FILL;
            }
            count = 0;
            glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    //释放
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}
