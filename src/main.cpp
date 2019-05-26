#include <cmath>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.hpp>
#include <camera.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double offsetX, double offsetY);

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); //Relative to world coordinate;
Camera myCamera(cameraPos);

float lastX = WIDTH / 2.f;
float lastY = HEIGHT / 2.f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrameTime = 0.0f;

glm::vec3 lightPos(1.2, 1.0f, 2.0f);

int main(){
    std::cout << "33:I am ok" << std::endl;
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

    // glViewport(0, 0, WIDTH, HEIGHT); 会在这里卡死，不知道为什么。

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader lightShader("F:/work-space/learnOpenGL_lite/src/shader/light_color_vertex.gc",
                    "F:/work-space/learnOpenGL_lite/src/shader/light_color_light_frag.gc");

    Shader objectShader("F:/work-space/learnOpenGL_lite/src/shader/light_color_vertex.gc",
                    "F:/work-space/learnOpenGL_lite/src/shader/ambient_frag.gc");

    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    unsigned VAO, VBO, lightVAO;
    glGenVertexArrays(1, &VAO);
    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;

    while(!glfwWindowShouldClose(window)){

        float currentFrameTime = glfwGetTime();
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        //输入
        processInput(window);

        //渲染
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        model = glm::mat4(1.0f);
        view = myCamera.getViewMatrix();
        projection = glm::perspective(glm::radians(myCamera.getZoom()), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        objectShader.use();
        objectShader.setFloatVec3("lightPos", lightPos);
        objectShader.setFloatMat4("model", model);
        objectShader.setFloatMat4("view", view);
        objectShader.setFloatMat4("projection", projection);
        objectShader.setFloatVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
        objectShader.setFloatVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lightShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2));
        lightShader.setFloatMat4("model", model);
        lightShader.setFloatMat4("view", view);
        lightShader.setFloatMat4("projection", projection);
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //检查并调用事件， 交换缓冲区
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        myCamera.processKeyboard(FORWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        myCamera.processKeyboard(BACKWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        myCamera.processKeyboard(LEFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        myCamera.processKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        myCamera.processKeyboard(RAISE, deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
        myCamera.processKeyboard(DROP, deltaTime);
    }

    //控制光源
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        lightPos.y += 0.05f;
    }

    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        lightPos.y -= 0.05f;
    }

    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        lightPos.x -= 0.05f;
    }

    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        lightPos.x += 0.05f;
    }

    if(glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS){
        lightPos.z += 0.05f;
    }

    if(glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS){
        lightPos.y -= 0.05f;
    }

}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float offsetX = xPos - lastX;
    float offsetY = lastY - yPos; 

    lastX = xPos;
    lastY = yPos;

    myCamera.processMouseMovement(offsetX, offsetY);
}

void scroll_callback(GLFWwindow* window, double offsetX, double offsetY)
{
    myCamera.processMouseScroll(offsetY);
}