#include "Shader.h"

const GLuint Shader::INVALID_SHADER = -1;

Shader::Shader(const std::string& _text, GLenum _type):text(_text), type(_type)
{
}

bool Shader::compile()
{
    GLuint tmpId = glCreateShader(type);

    if (tmpId == 0){
		lastError = "Error creating shader type";
        return false;
	}
    
	const GLchar* p[] = { text.c_str() };
	GLint length[] = { text.length() };

    glShaderSource(tmpId, 1, p, length);
    glCompileShader(tmpId);

    GLint res;
    glGetShaderiv(tmpId, GL_COMPILE_STATUS, &res);
    if (!res)
	{
        GLchar log[1024];
        glGetShaderInfoLog(tmpId, sizeof(log), NULL, log);
		lastError = log;
        return false;
	}
	id = tmpId;

	return true;
}
