#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
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
    void setFloatMat4(const std::string &name, glm::mat4 mat4) const;
    void setFloatVec3(const std::string &name, glm::vec3 vec3) const;
    void setFloatVec3(const std::string &name, float component1, float component2, float component3) const;
    unsigned int getID();
};