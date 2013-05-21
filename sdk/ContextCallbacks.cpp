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
	
	if (context.getIsInitTweakBars()){
		TwDraw();
	}

	glutSwapBuffers();	
}

void ContextCallbacks::reshape(int width, int height)
{
	GLContextGetter get(context);

	reshapeImpl(width, height);
	
	if (context.getIsInitTweakBars())
		TwWindowSize(width, height);
}

void ContextCallbacks::mouse(int button, int state, int x, int y)
{
	GLContextGetter get(context);
	if (context.getIsInitTweakBars()) {
		if (TwEventMouseButtonGLUT(button, state, x, y)) {
			//return;
		}
	}
	mouseImpl(button, state, x, y);
}

void ContextCallbacks::motion(int x, int y)
{
	GLContextGetter get(context);
	if (context.getIsInitTweakBars())
		if(TwEventMouseMotionGLUT(x, y)) {
			return;
		}
	motionImpl(x, y);
}

void ContextCallbacks::passiveMotion(int x, int y)
{
	GLContextGetter get(context);
	if (context.getIsInitTweakBars())
		TwEventMouseMotionGLUT(x, y);
	passiveMotionImpl(x, y);
}

void ContextCallbacks::keyboard(unsigned char key, int x, int y)
{
	GLContextGetter get(context);
	if (context.getIsInitTweakBars())
		TwEventKeyboardGLUT(key, x, y);
	keyboardImpl(key, x, y);
}

void ContextCallbacks::special(int key, int x, int y)
{
	GLContextGetter get(context);
	if (context.getIsInitTweakBars())
		TwEventSpecialGLUT(key, x, y);
	specialImpl(key, x, y);
}

void ContextCallbacks::close()
{
	GLContextGetter get(context);
	closeImpl();
}
