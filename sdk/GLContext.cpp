#include "GLContext.h"

#include <cassert>

#include "gl.h"

#include "ContextCallbacks.h"

const int GLContext::INVALID_CONTEXT = -1;
boost::unordered_map<int, GLContext::CallbackHolder> GLContext::Callbacks;

GLContextGetter::GLContextGetter(const GLContext& context):oldWindowId(glutGetWindow())
{
	assert(context.getWindowId() != GLContext::INVALID_CONTEXT);
	glutSetWindow(context.getWindowId());
}

GLContextGetter::~GLContextGetter()
{
	glutSetWindow(oldWindowId);
}

GLContext::GLContext(int _windowId):windowId(_windowId)
{
}

int GLContext::getWindowId() const
{
	return windowId;
}

void GLContext::registerCallbacks(ContextCallbacks* callbacks) const
{
	Callbacks[windowId] = CallbackHolder(callbacks);
	callbacks->setContext(*this);

	callbacks->init();

	GLContextGetter get(*this);
	glutDisplayFunc(GLContext::displayFunc);
	glutIdleFunc(GLContext::idleFunc);
}

GLContext GLContext::getCurrentContext()
{
	return GLContext(glutGetWindow());
}

void GLContext::displayFunc()
{
	GLContext context = getCurrentContext();
	auto it = Callbacks.find(context.getWindowId());
	if (it != Callbacks.end())
		it->second->display();
}

void GLContext::idleFunc()
{
	for (auto it = Callbacks.begin(); it != Callbacks.end(); ++it)
		if (it->second->hasIdleFunc())
			it->second->idle();
}