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
Object* mObject;

void sceneInit()
{
    mShader = new Shader("shaders/vsh_simple.glsl", "shaders/fsh_simple.glsl");

    mObject = new Object("models/utah.obj");
}

void sceneRender()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    mShader->use();

    // pass projection matrix to shader (note that in this case it could change every frame)
    mShader->setMat4("projection", glm::perspective(glm::radians(90.0f), (float) SCREEN_WIDTH / (float) SCREEN_HEIGHT, 0.1f, 100.0f));

    // camera/view transformation
    mShader->setMat4("view", glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp));

    // set color to red
    mShader->setVec3("color", glm::vec3(1.0, 0.0, 0.0));

    mObject->draw();
}

int main(void)
{
    windowInit(SCREEN_WIDTH, SCREEN_HEIGHT);
    sceneInit();

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