#pragma once

#include <string>
#include <vector>
#include "Shader.h"
#include "GLContext.h"

class ShaderProgram
{
public:
	ShaderProgram();
	ShaderProgram(const GLContext& _context);
	~ShaderProgram(void);

	void attachShader(const Shader& shader);
	bool compile();
	GLuint getId() const{ return id; }
	GLuint getUniformLoc(const std::string& name) const;
	void use() const;

	const std::string& getLastError() const{ return lastError; }

	static const GLuint INVALID_PROGRAM;
private:
	GLContext context;

	std::vector<GLuint> shaders;
	GLuint				id;

	std::string lastError;
};

