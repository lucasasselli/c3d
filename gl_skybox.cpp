#include <stdio.h>

#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <string>

#include "shader.h"
#include "object.h"
#include "window.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

Shader* skyboxShader;
Shader* phongShader;
Object* teapotObj;
Object* skyboxObj;

glm::vec3 lightPos(5.0, 10.0, -5.0);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

GLuint skyboxTex;

void sceneInit()
{
    skyboxShader = new Shader("shaders/vsh_simple.glsl", "shaders/vsh_skybox.glsl");
    phongShader = new Shader("shaders/vsh_phong.glsl", "shaders/fsh_phong.glsl");

    teapotObj = new Object("models/utah.obj");
    skyboxObj = new Object("models/cube.obj");

    std::vector<std::string> faces
    {
        "textures/skybox/venaria/px.png",
        "textures/skybox/venaria/nx.png",
        "textures/skybox/venaria/py.png",
        "textures/skybox/venaria/ny.png",
        "textures/skybox/venaria/pz.png",
        "textures/skybox/venaria/nz.png"
    };

    glGenTextures(1, &skyboxTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            printf("Error opening %s", faces[i].c_str());
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}

void sceneRender()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float) SCREEN_WIDTH / (float) SCREEN_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    // Skybox
    glDepthMask(GL_FALSE);
    skyboxShader->use();

    // ... set view and projection matrix
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

    skyboxShader->setMat4("projection", projection);
    skyboxShader->setMat4("view", glm::mat4(glm::mat3(view))); // Remove the traslation to view matrix to prevent the skybox from clipping!
    skyboxShader->setMat4("model", glm::mat4(1.0));

    skyboxShader->setInt("skybox", 0);

    skyboxObj->draw();
    glDepthMask(GL_TRUE);

    glm::mat4 model(1.0);
    model = glm::scale(model, glm::vec3(0.25));

    // Teapot
    phongShader->use();

    phongShader->setMat4("projection", projection);
    phongShader->setMat4("view", view);
    phongShader->setMat4("model", model);

    phongShader->setVec3("lightPos", lightPos);
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