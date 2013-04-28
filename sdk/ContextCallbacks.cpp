#include "ContextCallbacks.h"

#include "gl.h"

ContextCallbacks::ContextCallbacks():context(GLContext::INVALID_CONTEXT)
{
}

void ContextCallbacks::setContext(const GLContext& _context)
{
	context = _context;
}

void ContextCallbacks::init()
{
	GLContextGetter get(context);
	initImpl();
}

void ContextCallbacks::idle()
{
	GLContextGetter get(context);
	idleImpl();
	glutPostRedisplay();
}

void ContextCallbacks::display()
{
	GLContextGetter get(context);
	displayImpl();
}
