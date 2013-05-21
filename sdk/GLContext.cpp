#include "GLContext.h"

#include <cassert>

#include "gl.h"

#include "ContextCallbacks.h"

struct GLContextData
{
	Camera* camera;
};

const int GLContext::INVALID_CONTEXT = -1;
boost::unordered_map<int, GLContext::CallbackHolder> GLContext::Callbacks;
boost::unordered_map<int, GLContextData*> GLContext::Datas;

GLContextGetter::GLContextGetter(const GLContext& context):oldWindowId(glutGetWindow()), hasAntBar(context.getIsInitTweakBars())
{
	assert(context.getWindowId() != GLContext::INVALID_CONTEXT);
	glutSetWindow(context.getWindowId());
	if (hasAntBar)
		TwSetCurrentWindow(context.getWindowId());
}

GLContextGetter::~GLContextGetter()
{
	glutSetWindow(oldWindowId);
	if (hasAntBar)
		TwSetCurrentWindow(oldWindowId);
}

GLContext::GLContext(int _windowId):windowId(_windowId), isTweakBarsInit(false)
{
	if (
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
	glutReshapeFunc(GLContext::reshapeFunc);
	glutCloseFunc(GLContext::closeFunc);
	glutMouseFunc(GLContext::mouseFunc);
	glutMotionFunc(GLContext::motionFunc);
	glutPassiveMotionFunc(GLContext::passiveMotionFunc);
	glutKeyboardFunc(GLContext::keyboardFunc);
	glutSpecialFunc(GLContext::specialFunc);
	//atexit(GLContext::terminateFunc);
}

void GLContext::initTweakBars()
{
	GLContextGetter get(*this);
	assert(!isTweakBarsInit);
	isTweakBarsInit = true;
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

void GLContext::reshapeFunc(int width, int height)
{
	GLContext context = getCurrentContext();
	auto it = Callbacks.find(context.getWindowId());
	if (it != Callbacks.end())
		it->second->reshape(width, height);
}

void GLContext::mouseFunc(int button, int state, int x, int y)
{
	GLContext context = getCurrentContext();
	auto it = Callbacks.find(context.getWindowId());
	if (it != Callbacks.end())
		it->second->mouse(button, state, x, y);
}

void GLContext::motionFunc(int x, int y)
{
	GLContext context = getCurrentContext();
	auto it = Callbacks.find(context.getWindowId());
	if (it != Callbacks.end())
		it->second->motion(x, y);
}

void GLContext::passiveMotionFunc(int x, int y)
{
	GLContext context = getCurrentContext();
	auto it = Callbacks.find(context.getWindowId());
	if (it != Callbacks.end())
		it->second->passiveMotion(x, y);
}

void GLContext::keyboardFunc(unsigned char key, int x, int y)
{
	GLContext context = getCurrentContext();
	auto it = Callbacks.find(context.getWindowId());
	if (it != Callbacks.end())
		it->second->keyboard(key, x, y);
}

void GLContext::specialFunc(int key, int x, int y)
{
	GLContext context = getCurrentContext();
	auto it = Callbacks.find(context.getWindowId());
	if (it != Callbacks.end())
		it->second->keyboard(key, x, y);
}

void GLContext::closeFunc()
{
	GLContext context = getCurrentContext();
	auto it = Callbacks.find(context.getWindowId());
	if (it != Callbacks.end())
		it->second->close();
	Callbacks.erase(it);
	if (Callbacks.empty())
		TwTerminate();
}

