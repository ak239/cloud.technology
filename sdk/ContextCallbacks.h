#pragma once

#include "GLContext.h"

/**
* Implements this abstract class to do callbacks in context
* init, idle and display algo: get context -> call impl
*/
class ContextCallbacks
{
public:
	ContextCallbacks();
	virtual ~ContextCallbacks(){}
	// Guaranted, that context will be setted by GLContext::setCallbacks before other calls
	void setContext(const GLContext& _context);
	const GLContext& getContext() const{ return context; }

	void init();
	
	// Guaranted glutPostRedisplay() call after idle
	void idle();

	void display();

	void reshape(int width, int height);

	// Input processing
	void mouse(int button, int state, int x, int y);
	void motion(int x, int y);
	void passiveMotion(int x, int y);
	void keyboard(unsigned char key, int x, int y);
	void special(int key, int x, int y);

	// Guaranted call TwTerminate if context init tweak bars
	void close();

	virtual bool hasIdleFunc(){ return false; }
private:
	virtual void initImpl(){}
	virtual void idleImpl(){}
	virtual void displayImpl() = 0;
	virtual void reshapeImpl(int width, int height){}

	virtual void mouseImpl(int button, int state, int x, int y){}
	virtual void motionImpl(int x, int y){}
	virtual void passiveMotionImpl(int x, int y){}
	virtual void keyboardImpl(unsigned char key, int x, int y){}
	virtual void specialImpl(int key, int x, int y){}

	virtual void closeImpl(){};

	GLContext context;
};

