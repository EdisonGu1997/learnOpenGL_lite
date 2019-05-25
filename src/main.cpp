#include <cmath>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
glm::mat4 trans;
int main(){
    const unsigned int WIDTH = 800;
    const unsigned int HEIGHT = 600;
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGl-lite", NULL, NULL);
    if(window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Shader myShader("F:/work-space/learnOpenGL_lite/src/shader/trans_vertex.gc","F:/work-space/learnOpenGL_lite/src/shader/tex_fragment.gc");

    float vertices[] = {
        //位置                //颜色             //纹理坐标
         0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        3, 2, 0,
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //创建VAO之后，设置顶点数据信息
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
                            8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //贴图位置信息
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 
                            8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);


    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    // std::cout << "load";
    unsigned char *data = 
        stbi_load(
            "F:/work-space/learnOpenGL_lite/res/img/lz.jpg", 
            &width, &height, &nrChannels, 0);

    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
                    width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
    }else{
        std::cout << "Failed to load texture"
                  << std::endl;
    }
    stbi_image_free(data);
    myShader.use();
    //查询可用的顶点属性数量
    // int nrAttrib;
    // glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttrib);
    // std::cout << "Maximum nr of vertex attribute: "
    //           << nrAttrib
    //           << std::endl; 

    //旋转和缩放
    
    //沿Z轴
    // trans = glm::rotate(trans, glm::radians(90.f), glm::vec3(0.0, 0.0, 1.0));
    // trans = glm::scale(trans, glm::vec3(1.f, 1.f, 1.f));
    trans = glm::translate(trans, glm::vec3(0.2f, 0.2f, 0.0f));

    unsigned int transformLoc = glGetUniformLocation(myShader.getID(), "transform");
    std::cout << glfwGetTime();
    float angle = 0.0f;

    while(!glfwWindowShouldClose(window)){
        //输入
        processInput(window);

        //渲染
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glBindTexture(GL_TEXTURE_2D, texture);
        myShader.use();
        glBindVertexArray(VAO);

        angle += 0.0001f;
        trans = glm::rotate(trans, angle, glm::vec3(0.f, 0.f, 1.f));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //检查并调用事件， 交换缓冲区
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }else if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        trans = glm::translate(trans, glm::vec3(-0.1, 0, 0));
    }
}