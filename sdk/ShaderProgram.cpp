#include "ShaderProgram.h"

#include <cassert>

const GLuint ShaderProgram::INVALID_PROGRAM = 0;

ShaderProgram::ShaderProgram():context(GLContext::INVALID_CONTEXT), id(INVALID_PROGRAM)
{
}


ShaderProgram::ShaderProgram(const GLContext& _context):context(_context), id(INVALID_PROGRAM)
{
}


ShaderProgram::~ShaderProgram(void)
{
}


void ShaderProgram::attachShader(const Shader& shader)
{
	assert(shader.getId() != Shader::INVALID_SHADER);
	shaders.push_back(shader.getId());
}


bool ShaderProgram::compile()
{
	GLContextGetter get(context);

	GLuint tmpId = glCreateProgram();

	if (tmpId == 0)
	{
		lastError = "Can't create program";
        return false;
    }

	for (unsigned i = 0; i < shaders.size(); ++i)
		glAttachShader(tmpId, shaders[i]);

	GLint  res = 0;
    GLchar log[1024] = { 0 };

    glLinkProgram(tmpId);
    glGetProgramiv(tmpId, GL_LINK_STATUS, &res);
	if (res == 0)
	{
		glGetProgramInfoLog(tmpId, sizeof(log), NULL, log);
		lastError = "Program link error: " + std::string(log);
        return false;
	}

    glValidateProgram(tmpId);
    glGetProgramiv(tmpId, GL_VALIDATE_STATUS, &res);
    if (!res) 
	{
		glGetProgramInfoLog(tmpId, sizeof(log), NULL, log);
        lastError = "Program validate error: " + std::string(log);
        return false;
    }

	id = tmpId;
	return true;
}

GLuint ShaderProgram::getUniformLoc(const std::string& name) const
{
	assert(id != INVALID_PROGRAM);
	GLuint loc =glGetUniformLocation(id, name.c_str());
	//assert(loc != 0xFFFFFFFF);
	return loc;
}

void ShaderProgram::use() const
{
	assert(id != INVALID_PROGRAM);
	GLContextGetter get(context);
	glUseProgram(id);
}
