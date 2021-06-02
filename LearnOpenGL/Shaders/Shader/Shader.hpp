//
//  Shader.hpp
//  Shader
//  用C++封装的Shader类可以从硬盘读取着色器，然后编译并链接它们，并对它们进行错误检测。
//
//  Created by apophis on 2021/5/31.
//

#ifndef Shader_hpp
#define Shader_hpp

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
