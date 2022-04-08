#include "input.h"

// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

Delta<float> timeDelta;
Delta<float> xposDelta;
Delta<float> yposDelta;

float yaw = -90.0;
float pitch = 0.0;

static void inputMouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    xposDelta.sample(static_cast<float>(xpos));
    yposDelta.sample(static_cast<float>(ypos));

    yaw += xposDelta.get()*INPUT_MOUSE_SENSITIVITY;
    pitch -= yposDelta.get()*INPUT_MOUSE_SENSITIVITY;

    pitch = glm::clamp(pitch, -89.0f, +89.0f);

    // printf("xpos = %0lf ypos = %0lf\n", xpos, ypos);
    // printf("pitch = %0f yaw = %0f\n", pitch, yaw);

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

static void inputKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_Q)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void inputParse(GLFWwindow* window)
{
    timeDelta.sample(glfwGetTime());

    float cameraSpeed = INPUT_CAMERA_SPEED * timeDelta.get();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void inputInit(GLFWwindow* window)
{
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Mouse callback
    glfwSetCursorPosCallback(window, inputMouseCallback);

    // Key callback
    glfwSetKeyCallback(window, inputKeyCallback);
}