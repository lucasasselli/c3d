#include <stdio.h>

#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#define GLM_SWIZZLE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "object.h"
#include "window.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

Shader* simpleShader;
Shader* phongShader;
Object* teapotObj;
Object* cubeObj;

glm::vec3 lightPos(5.0, 10.0, -5.0);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

void sceneInit()
{
    simpleShader = new Shader("shaders/vsh_phong.glsl", "shaders/fsh_simple.glsl");
    phongShader = new Shader("shaders/vsh_phong.glsl", "shaders/fsh_phong.glsl");

    teapotObj = new Object("models/utah.obj");
    cubeObj = new Object("models/cube.obj");
}

void sceneRender()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float) SCREEN_WIDTH / (float) SCREEN_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    // Cube
    simpleShader->use();

    glm::vec4 light(0.0f, 0.0f, 0.0f, 1.0f);

    glm::mat4 model(1.0);
    model = glm::rotate(model, (float) glfwGetTime()*0.8f, glm::vec3(0, 1 , 0));
    model = glm::translate(model, lightPos);

    light = model * light;

    simpleShader->setMat4("projection", projection);
    simpleShader->setMat4("view", view);
    simpleShader->setMat4("model", model);

    simpleShader->setVec3("color", glm::vec3(1.0f));

    cubeObj->draw();

    // Teapot
    phongShader->use();

    phongShader->setMat4("projection", projection);
    phongShader->setMat4("view", view);
    phongShader->setMat4("model", glm::mat4(1.0f));

    phongShader->setVec3("lightPos", light.xyz());
    phongShader->setVec3("lightColor", lightColor);
    phongShader->setVec3("cameraPos", cameraPos);

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