#include "object.h"

Object::Object(const char* path)
{
    char a;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &NBO);

    FILE* f = fopen(path, "r");
	if (!f)
	{
		printf("Could not open object: %s\n", path);
		return;
	}

    float temp[3] = {0};

    while (fscanf(f, "%c %f %f %f\n", &a, &temp[0], &temp[1], &temp[2]) != EOF)
    {
        switch (a)
        {
            case 'v':
                // Vertex
                for(int i=0; i<3; i++){
                    vertices.push_back(temp[i]);
                }
                break;

            case 'f':
                // Face
                for(int i=0; i<3; i++){
                    indices.push_back(temp[i]-1); // .obj index start from 1!
                }
                break;
        }
    }

    for(int i=0; i<vertices.size()/3; i++)
    {
        glm::vec3 n = glm::vec3(0.0f, 0.0f, 0.0f);

        for(int j=0; j<indices.size()/3; j++)
        {
            bool found = false;

            glm::ivec3 I = get(&indices, j);

            // Check if vertex belongs to triangle
            for(int k=0; k<3; k++)
            {
                if(i == I[k])
                    found = true;
            }

            if(found)
            {
                glm::vec3 A = get(&vertices, I.x);
                glm::vec3 B = get(&vertices, I.y);
                glm::vec3 C = get(&vertices, I.z);

                n += glm::cross(B-A, C-A);
            }
        }

        n = glm::normalize(n);

        normals.push_back(8*n.x);
        normals.push_back(8*n.y);
        normals.push_back(8*n.z);
    }

    use();

    // copy our index array in a element buffer for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLint), &indices[0], GL_STATIC_DRAW);

    // then set the vertex attributes pointers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(GLfloat), &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(1);
}

Object::~Object()
{
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void Object::use()
{
    glBindVertexArray(VAO);
}

void Object::draw()
{
    use();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

glm::ivec3 Object::get(std::vector<GLint>* v, int i)
{
    return glm::ivec3((*v)[i*3+0], (*v)[i*3+1], (*v)[i*3+2]);
}

glm::vec3 Object::get(std::vector<GLfloat>* v, int i)
{
    return glm::vec3((*v)[i*3+0], (*v)[i*3+1], (*v)[i*3+2]);
}