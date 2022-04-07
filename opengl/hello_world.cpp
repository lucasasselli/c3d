#include <stdio.h>

#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "object.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

GLdouble cx = 0.0f;
GLdouble cy = 0.0f;
GLdouble zoom = 1.0f;

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

double oldTime = 0;

static void handleGamepad(GLFWwindow* window, const GLFWgamepadstate& gamepad)
{
	double curTime = glfwGetTime();
	GLfloat deltaTime = curTime - oldTime;
    oldTime = curTime;

    GLfloat k = deltaTime/0.1;

	const bool left_pressed = gamepad.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT] == GLFW_PRESS;
	const bool right_pressed = gamepad.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] == GLFW_PRESS;
	const bool up_pressed = gamepad.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP] == GLFW_PRESS;
	const bool down_pressed = gamepad.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN] == GLFW_PRESS;

	const bool zoomin_pressed = gamepad.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER] == GLFW_PRESS;
	const bool zoomout_pressed = gamepad.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER] == GLFW_PRESS;

	const bool exit_pressed  = gamepad.buttons[GLFW_GAMEPAD_BUTTON_START] == GLFW_PRESS;

    if (exit_pressed)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if (up_pressed && cy < 1.0)
        cy += 0.1*zoom*k;

    if (down_pressed && cy > -1.0)
        cy -= 0.1*zoom*k;

    if (right_pressed && cx < 1.0)
        cx += 0.1*zoom*k;

    if (left_pressed && cx > -2.0)
        cx -= 0.1*zoom*k;

    if (zoomout_pressed && zoom < 1.0f)
        zoom *= (1.0f + k*0.5f);

    if (zoomin_pressed)
        zoom /= (1.0f + k*0.5f);
}

static GLuint VAO, VBO, EBO;

Shader* mShader;
Object* mObject;

void sceneInit()
{
    mShader = new Shader("shaders/vsh_simple.glsl", "shaders/fsh_one_color.glsl");
    mObject = new Object("../models/utah.obj");

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

// camera
// FIXME: Create a camera object
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

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
    glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_FALSE);
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

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    gladLoadGL();
    glfwSwapInterval(1);

    // Start rendering
    sceneInit();

	GLFWgamepadstate gamepad = {};

    while (!glfwWindowShouldClose(window))
    {
        // Read gamepad
		if (!glfwGetGamepadState(GLFW_JOYSTICK_1, &gamepad))
		{
			// Gamepad not available, so let's fake it with keyboard
			gamepad.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT]  = glfwGetKey(window, GLFW_KEY_A);
			gamepad.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] = glfwGetKey(window, GLFW_KEY_D);
			gamepad.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP]    = glfwGetKey(window, GLFW_KEY_W);
			gamepad.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN]  = glfwGetKey(window, GLFW_KEY_S);

			gamepad.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER] = glfwGetKey(window, GLFW_KEY_E);
			gamepad.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER]  = glfwGetKey(window, GLFW_KEY_Q);

			gamepad.buttons[GLFW_GAMEPAD_BUTTON_START] = glfwGetKey(window, GLFW_KEY_ESCAPE);
		}
        handleGamepad(window, gamepad);

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