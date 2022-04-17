#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <glad/glad.h>

#include <stdio.h>

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Object
{

public:
    std::vector<GLfloat> vertices;
    std::vector<GLint> indices;
    std::vector<GLfloat> normals;

    Object(const char*);
    ~Object();
    void use();
    void draw();

private:
    GLuint VAO, VBO, EBO, NBO;

    glm::ivec3 get(std::vector<GLint>*, int);
    glm::vec3 get(std::vector<GLfloat>*, int);

};

#endif