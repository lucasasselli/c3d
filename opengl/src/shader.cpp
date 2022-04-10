#include "shader.h"

Shader::Shader(const char* vsh_path, const char* fsh_path)
{
    GLint vsh = load(GL_VERTEX_SHADER, vsh_path);
    GLint fsh = load(GL_FRAGMENT_SHADER, fsh_path);

    program = glCreateProgram();

    glAttachShader(program, vsh);
    glAttachShader(program, fsh);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLchar msg[512];
        glGetProgramInfoLog(program, sizeof(msg), nullptr, msg);
        printf("Shader link %s", msg);
    }

    glDeleteShader(vsh);
    glDeleteShader(fsh);
}

Shader::~Shader()
{
    glDeleteProgram(program);
}

void Shader::use()
{
    glUseProgram(program);
}

void Shader::setVec3(const char* name, glm::vec3 a)
{
    GLuint loc = glGetUniformLocation(program, name);
    glUniform3f(loc, a[0], a[1], a[2]);
}

void Shader::setMat4(const char* name, glm::mat4 a)
{
    GLuint loc = glGetUniformLocation(program, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, &a[0][0]);
}

GLint Shader::load(GLenum type, const char* path)
{
	FILE* f = fopen(path, "rb");
	if (!f)
	{
		printf("Could not open shader: %s\n", path);
		return 0;
	}

	fseek(f, 0, SEEK_END);
	size_t shader_size = ftell(f);
	rewind(f);

	char* source = new char[shader_size+1];
	fread(source, 1, shader_size, f);
	source[shader_size] = 0;
	fclose(f);

    GLint handle = glCreateShader(type);
    if (!handle)
    {
        printf("%u: cannot create shader", type);
        return 0;
    }
    glShaderSource(handle, 1, &source, nullptr);
	delete[] source;

    glCompileShader(handle);

    GLint success;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar msg[512];
        glGetShaderInfoLog(handle, sizeof(msg), nullptr, msg);
        printf("%s: %s\n", path, msg);
        glDeleteShader(handle);
        return 0;
    }

    return handle;
}