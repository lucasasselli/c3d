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

static GLuint VAO, VBO, EBO;

Shader* mShader;
Object* mObject;

void sceneInit()
{
    mShader = new Shader("opengl/shaders/vsh_simple.glsl", "opengl/shaders/fsh_one_color.glsl");
    mObject = new Object("models/utah.obj");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind Vertex Array Object
    glBindVertexArray(VAO);

    // copy our vertices array in a vertex buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mObject->verticesCnt*sizeof(GLfloat), mObject->vertices, GL_STATIC_DRAW);

    // copy our index array in a element buffer for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mObject->indicesCnt*sizeof(GLint), mObject->indices, GL_STATIC_DRAW);

    // then set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
}

void sceneRender()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    mShader->use();

    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float) SCREEN_WIDTH / (float) SCREEN_HEIGHT, 0.1f, 100.0f);
    mShader->setMat4("projection", projection);

    // camera/view transformation
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    mShader->setMat4("view", view);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, mObject->indicesCnt, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void sceneExit()
{
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
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

    sceneExit();
    windowExit();

    exit(EXIT_SUCCESS);
}