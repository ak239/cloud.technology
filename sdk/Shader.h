#pragma once

#include <string>
#include "gl.h"

class Shader
{
public:
	Shader(const std::string& _text, GLenum _type);
	~Shader(void){}

	bool compile();

	const std::string& getLastError() const{ return lastError; }
	GLuint getId() const{ return id; }

	static const GLuint INVALID_SHADER; 
private:
	std::string text;
	GLenum      type;
	GLuint      id;

	std::string lastError;
};

