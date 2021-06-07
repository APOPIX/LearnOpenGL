//
//  ShadersExercise3.cpp
//  ShadersEx.3
//
//  课后习题3: 使用out关键字把顶点位置输出到片段着色器，并将片段的颜色设置为与顶点位置相等
//           （来看看连顶点位置值都在三角形中被插值的结果）。
//           做完这些后，尝试回答下面的问题：为什么在三角形的左下角是黑的?
//
//           答：
//           三角形的左下角为黑色是因为第三象限的顶点中，x、y 为负数，z 为 0，显示颜色时，负数按 0 处理。
//           因此左下角的颜色相当于 (0, 0, 0, 0)，即黑色*/
//
//  Created by apophis on 2021/6/7.
//

#include "Shader/Shader.hpp"
#include <iostream>
#include "glad.h"
#include "glfw3.h"
#include <thread>
#include <chrono>

//监听窗口尺寸改变
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

float vertices[] = {
    // 位置
     0.5f, -0.5f, 0.0f, // 右下
    -0.5f, -0.5f, 0.0f, // 左下
     0.0f,  0.5f, 0.0f, // 顶部
};

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
    
    //重头戏：使用我们的Shader类
    Shader shader("./shadersExercise3.vert", "./shadersExercise3.frag");
    
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);//绑定到当前 GL 环境
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // 位置属性
    // 因为去掉了颜色，所以 stride = 3 个 float 大小
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    shader.use();
    
    // 练习2:设置偏移量将三角形移动到右边
    shader.setFloat("offset", 0.4);
    
    while(!glfwWindowShouldClose(window))
    {//glfwWindowShouldClose函数在我们每次循环的开始前检查一次GLFW是否被要求退出
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//设置以何种颜色来清空屏幕
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除颜色缓冲
        //绑定VAO
        glBindVertexArray(VAO);
        //绘制三角形
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwPollEvents();
        glfwSwapBuffers(window);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    //释放
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    //glDeleteProgram会在Shader析构函数中自动执行
}
