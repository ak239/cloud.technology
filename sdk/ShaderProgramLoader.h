#pragma once

#include <string>
#include "ShaderProgram.h"
#include "GLContext.h"

class ShaderProgramLoader
{
public:
	virtual ~ShaderProgramLoader(void){}

	bool loadProgram(const GLContext& context);
	const ShaderProgram& getProgram() const{ return program; }

	bool hasError() const{ return !lastError.empty(); }
	const std::string& getLastError() const{ return lastError; }

protected:
	void setLastError(const std::string& _lastError)
	{
		lastError = _lastError;
	}

	void setProgram(const ShaderProgram& _program)
	{
		program = _program;
	}

private:
	virtual bool loadProgramImpl(const GLContext& context) = 0;

	ShaderProgram program;
	std::string   lastError;
};

