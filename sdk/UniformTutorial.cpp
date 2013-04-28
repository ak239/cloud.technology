#include "UniformTutorial.h"

#include <iostream>
#include <cassert>

namespace{
const char* pVS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
layout (location = 0) in vec3 Position;                                             \n\
                                                                                    \n\
uniform float gScale;                                                               \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    gl_Position = vec4(gScale * Position.x, gScale * Position.y, Position.z, 1.0);  \n\
}";

const char* pFS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
out vec4 FragColor;                                                                 \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);                                           \n\
}";
}

void UniformTutorial::addShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
    GLuint shaderObj = glCreateShader(ShaderType);

    if (shaderObj == 0){
		std::cerr << "Error creating shader type " << ShaderType << std::endl;
        return;
	}
    
    const GLchar* p[1];
    p[0] = pShaderText;
    GLint Lengths[1];
    Lengths[0]= strlen(pShaderText);
    glShaderSource(shaderObj, 1, p, Lengths);
    glCompileShader(shaderObj);
    GLint success;
    glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);
    if (!success)
	{
        GLchar InfoLog[1024];
        glGetShaderInfoLog(shaderObj, 1024, NULL, InfoLog);
		std::cerr << "Error compiling shader type " << ShaderType << " : " << InfoLog << std::endl;
        return;
	}
    glAttachShader(ShaderProgram, shaderObj);
}

void UniformTutorial::compileShader()
{
    GLuint ShaderProgram = glCreateProgram();

    if (ShaderProgram == 0) {
		std::cerr << "Error creating shader program\n" << std::endl;
        return;
    }

    addShader(ShaderProgram, pVS, GL_VERTEX_SHADER);
    addShader(ShaderProgram, pFS, GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

    glLinkProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
        return;
	}

    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
        return;
    }

    glUseProgram(ShaderProgram);

	gScaleLocation = glGetUniformLocation(ShaderProgram, "gScale");
	assert(gScaleLocation != 0xFFFFFFFF);
}