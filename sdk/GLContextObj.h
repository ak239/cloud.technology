#pragma once

#include "GLContext.h"

class GLContextObj
{
public:
	GLContextObj() : context(GLContext::getCurrentContext()){}
	GLContextObj(const GLContext& _context) : context(_context){}

	const GLContext& getContext() const{ return context; }

private:
	GLContext context;
};

