#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <glad/glad.h>

#include <stdio.h>

class Object{

public:
    GLfloat* vertices;
    GLint* indices;

    GLint verticesCnt;
    GLint indicesCnt;

    Object(const char*);
    ~Object();
};

#endif