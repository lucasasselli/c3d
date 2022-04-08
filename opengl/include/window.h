#ifndef _INPUT_H_
#define _INPUT_H_

#include <stdio.h>

#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "delta.h"

#define INPUT_MOUSE_SENSITIVITY 0.4f
#define INPUT_CAMERA_SPEED      6.0f

extern glm::vec3 cameraPos;
extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;

extern GLFWwindow* window;

static void windowMouseCallback(GLFWwindow*, double, double);

static void windowErrorCallback(int, const char*);

static void windowSizeCallback(GLFWwindow*, int, int);

void windowInit(int, int);

void windowExit();

void inputParse();

#endif