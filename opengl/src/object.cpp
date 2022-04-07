#include "object.h"

Object::Object(const char* path)
{
    char a;
    float x, y, z;

	FILE* f = fopen(path, "r");
	if (!f)
	{
		printf("Could not open object: %s\n", path);
		return;
	}

    verticesCnt = 0;
    indicesCnt = 0;

    // Count
    while (fscanf(f, "%c %f %f %f\n", &a, &x, &y, &z) != EOF)
    {
        switch (a)
        {
            case 'v':
                // Vertex
                verticesCnt+=3;
                break;

            case 'f':
                // Face
                indicesCnt+=3;
                break;
        }
    }

	rewind(f);

    indices = new GLint[indicesCnt+1];
    vertices = new GLfloat[verticesCnt+1];

    verticesCnt = 0;
    indicesCnt = 0;

    // Read object file
    while (fscanf(f, "%c %f %f %f\n", &a, &x, &y, &z) != EOF)
    {
        switch (a)
        {
            case 'v':
                // Vertex
                vertices[verticesCnt++] = x;
                vertices[verticesCnt++] = y;
                vertices[verticesCnt++] = z;
                break;

            case 'f':
                // Face
                indices[indicesCnt++] = x-1;
                indices[indicesCnt++] = y-1;
                indices[indicesCnt++] = z-1;
                break;
        }
    }
}

Object::~Object()
{
    delete[] vertices;
    delete[] indices;
}