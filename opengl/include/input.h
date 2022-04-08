#ifndef _INPUT_H_
#define _INPUT_H_

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "delta.h"

#define INPUT_MOUSE_SENSITIVITY 0.1f
#define INPUT_CAMERA_SPEED      0.9f

extern glm::vec3 cameraPos;
extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;

static void inputMouseCallback(GLFWwindow* window, double xpos, double ypos);

static void inputKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

void inputInit(GLFWwindow* window);

void inputParse(GLFWwindow* window);

#endif