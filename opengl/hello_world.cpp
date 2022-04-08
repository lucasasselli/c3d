#include <stdio.h>

#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "object.h"
#include "input.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

GLfloat ratio = GLfloat(SCREEN_HEIGHT)/GLfloat(SCREEN_WIDTH);

static void errorCallback(int error, const char* description)
{
    printf("Error: %s\n", description);
    exit(EXIT_FAILURE);
}

static void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	if (!width || !height)
		return;

    ratio = GLfloat(height)/GLfloat(width);

	glViewport(0, 0, width, height);
}

static GLuint VAO, VBO, EBO;

Shader* mShader;
Object* mObject;

void sceneInit()
{
    mShader = new Shader("opengl/shaders/vsh_simple.glsl", "opengl/shaders/fsh_one_color.glsl");
    mObject = new Object("models/cube.obj");

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
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
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
    // deviceInit();

    // Init GLFW
    // glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_FALSE);
    glfwSetErrorCallback(errorCallback);

    if (!glfwInit())
        exit(EXIT_FAILURE);


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "hello_world", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    inputInit(window);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    gladLoadGL();
    glfwSwapInterval(1);

    // Start rendering
    sceneInit();

    while (!glfwWindowShouldClose(window))
    {
        inputParse(window);

        sceneRender();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Terminate graphics
    // sceneExit();
    glfwDestroyWindow(window);
    glfwTerminate();

    // Terminate device
    // deviceStop();

    exit(EXIT_SUCCESS);
}