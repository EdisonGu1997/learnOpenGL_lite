#include <shader.hpp>

Shader::Shader(const char *vertexPath, const char *fragmentPath){
    //1.从文件中读取着色器源码
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try{
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }catch(std::ifstream::failure e){
        std::cout << "ERROR:Shader read shader source\n\t"
                  << "File not read successfully!"
                  <<std::endl;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    //编译着色器
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    //如果错误，打印错误信息
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR:Shader compile vertex shader\n\t"
                  << infoLog
                  << std::endl;
    }
    
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR:Shader compile fragment shader\n\t"
                  << infoLog
                  << std::endl;
    }

    _id = glCreateProgram();
    glAttachShader(_id, vertex);
    glAttachShader(_id, fragment);
    glLinkProgram(_id);
    
    glGetProgramiv(_id, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(_id, 512, NULL, infoLog);
        std::cout << "ERROR:Shader link program\n\t"
                  << infoLog
                  << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use(){
    glUseProgram(_id);
}

void Shader::unuse(){
    glUseProgram(0);
}

void Shader::setBool(const std::string &name, bool value) const{
    glUniform1i(glGetUniformLocation(_id, name.c_str()), (int)value);
}

void Shader::setFloat(const std::string &name, float value) const{
    glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
}

void Shader::setInt(const std::string &name, int value) const{
    glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
}

void Shader::setFloatMat4(const std::string &name, glm::mat4 mat4) const{
    glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, &mat4[0][0]);
}

void Shader::setFloatVec3(const std::string &name, glm::vec3 vec3) const{
    glUniform3fv(glGetUniformLocation(_id, name.c_str()), 1, &vec3[0]);
}

unsigned int Shader::getID(){
    return _id;
}