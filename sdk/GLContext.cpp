#include "GLContext.h"

#include <cassert>

#include "gl.h"

#include "ContextCallbacks.h"
#include "PersProjInfo.h"

struct GLContextData
{
	GLContextData():camera(0), isTweakBarInit(false), callbacks(0), projInfo(0){}
	Camera* camera;
	bool    isTweakBarInit;
		
	ContextCallbacks* callbacks;
	PersProjInfo*     projInfo;

	~GLContextData(){
		delete camera;
		delete projInfo;
	}
};

const int GLContext::INVALID_CONTEXT = -1;
std::vector<GLContextData*> GLContext::VecDatas(128);

/*
* Context Getter
*/
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

/*
* 
*/
GLContext::GLContext(int _windowId):windowId(_windowId)
{
	if (windowId == INVALID_CONTEXT){
		data = 0;
		return;
	}

	assert(static_cast<int>(VecDatas.size()) > windowId);
	if (!VecDatas[windowId]){
		data = new GLContextData();
		VecDatas[windowId] = data;
	}else
		data = VecDatas[windowId];
}

int GLContext::getWindowId() const
{
	return windowId;
}

void GLContext::registerCallbacks(ContextCallbacks* callbacks) const
{
	data->callbacks = callbacks;
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
}

void GLContext::initTweakBars()
{
	GLContextGetter get(*this);
	assert(!data->isTweakBarInit);
	data->isTweakBarInit = true;
}

bool GLContext::getIsInitTweakBars() const
{
	return data->isTweakBarInit;
}

void GLContext::setCamera(Camera* _camera)
{
	data->camera = _camera;
}

Camera* GLContext::getCamera() const
{
	return data->camera;
}

void GLContext::setPersProjInfo(PersProjInfo* _info)
{
	data->projInfo = _info;
}

PersProjInfo* GLContext::getPersProjInfo() const
{
	return data->projInfo;
}

GLint GLContext::getWidth() const
{
	GLContextGetter get(*this);
	return glutGet(GLUT_WINDOW_WIDTH);
}

GLint GLContext::getHeight() const
{
	GLContextGetter get(*this);
	return glutGet(GLUT_WINDOW_HEIGHT);
}

GLContext GLContext::getCurrentContext()
{
	return GLContext(glutGetWindow());
}

void GLContext::displayFunc()
{
	GLContext context = getCurrentContext();
	if (context.data->callbacks)
		context.data->callbacks->display();
}

void GLContext::idleFunc()
{
	for (std::size_t i = 0; i < VecDatas.size(); ++i)
		if (VecDatas[i] && VecDatas[i]->callbacks->hasIdleFunc())
			VecDatas[i]->callbacks->idle();
}

void GLContext::reshapeFunc(int width, int height)
{
	GLContext context = getCurrentContext();
	if (context.data->callbacks)
		context.data->callbacks->reshape(width, height);
}

void GLContext::mouseFunc(int button, int state, int x, int y)
{
	GLContext context = getCurrentContext();
	if (context.data->callbacks)
		context.data->callbacks->mouse(button, state, x, y);
}

void GLContext::motionFunc(int x, int y)
{
	GLContext context = getCurrentContext();
	if (context.data->callbacks)
		context.data->callbacks->motion(x, y);
}

void GLContext::passiveMotionFunc(int x, int y)
{
	GLContext context = getCurrentContext();
	if (context.data->callbacks)
		context.data->callbacks->passiveMotion(x, y);
}

void GLContext::keyboardFunc(unsigned char key, int x, int y)
{
	GLContext context = getCurrentContext();
	if (context.data->callbacks)
		context.data->callbacks->keyboard(key, x, y);
}

void GLContext::specialFunc(int key, int x, int y)
{
	GLContext context = getCurrentContext();
	if (context.data->callbacks)
		context.data->callbacks->keyboard(key, x, y);
}

void GLContext::closeFunc()
{
	GLContext context = getCurrentContext();
	if (context.data->callbacks)
		context.data->callbacks->close();

	VecDatas[context.windowId] = 0;
}

