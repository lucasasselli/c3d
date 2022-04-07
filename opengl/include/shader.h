#ifndef _SHADER_H_
#define _SHADER_H_

#include <stdio.h>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {

public:
    Shader(const char*, const char*);
    ~Shader();

    void setUniform1i(const char*, GLint);
    void setMat4(const char*, glm::mat4);

    void use();

private:
    GLuint program;

    GLint load(GLenum, const char*);

};

#endif