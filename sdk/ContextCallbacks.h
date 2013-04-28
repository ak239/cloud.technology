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

	void init();
	
	// Guaranted glutPostRedisplay() call after idle
	void idle();

	void display();

	virtual bool hasIdleFunc(){ return false; }
private:
	virtual void initImpl(){}
	virtual void idleImpl(){}
	virtual void displayImpl() = 0;

	GLContext context;
};

