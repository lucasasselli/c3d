#include <stdio.h>

#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "object.h"
#include "window.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

Shader* mShader;
Object* teapotObj;
Object* cubeObj;

glm::vec3 lightPos(5.0, 10.0, -5.0);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

void sceneInit()
{
    mShader = new Shader("opengl/shaders/vsh_phong.glsl", "opengl/shaders/fsh_phong.glsl");

    teapotObj = new Object("models/utah.obj");

    cubeObj = new Object("models/cube.obj");
}

void sceneRender()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mShader->use();

    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float) SCREEN_WIDTH / (float) SCREEN_HEIGHT, 0.1f, 100.0f);
    mShader->setMat4("projection", projection);

    // camera/view transformation
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    mShader->setMat4("view", view);

    // Cube
    mShader->setMat4("model", glm::translate(glm::mat4(1.0f), lightPos));
    cubeObj->draw();

    // Teapot
    mShader->setVec3("lightPos", lightPos);
    mShader->setVec3("lightColor", lightColor);
    mShader->setMat4("model", glm::mat4(1.0f));
    teapotObj->draw();

}

int main(void)
{
    windowInit(SCREEN_WIDTH, SCREEN_HEIGHT);
    sceneInit();

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        inputParse();

        sceneRender();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    windowExit();

    exit(EXIT_SUCCESS);
}