#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader{
    private:
    unsigned int _id;
    public:
    Shader(const GLchar *vertexPath, const GLchar *fragmentPath);
    void use();
    void unuse();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setFloatMat4(const std::string &name, const float *data_ptr) const;
    unsigned int getID();
};