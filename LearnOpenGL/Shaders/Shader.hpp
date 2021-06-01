//
//  Shader.hpp
//  Shaders
//  编写、编译、管理着色器是件麻烦事。在着色器主题的最后，我们会写一个类来让我们的生活轻松一点，
//  它可以从硬盘读取着色器，然后编译并链接它们，并对它们进行错误检测，这就变得很好用了。
//  这也会让你了解该如何封装目前所学的知识到一个抽象对象中。
//
//  Created by apophis on 2021/5/31.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <stdio.h>
#include <string>

class Shader {
public:
    // 程序ID
    unsigned int ID;

    // 构造器读取并构建着色器
    Shader(const char* vertexPath, const char* fragmentPath);
    //释放资源（glDeleteProgram）
    ~Shader();
    // 使用/激活程序
    void use();
    
    // uniform工具函数
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
};

#endif /* Shader_hpp */
