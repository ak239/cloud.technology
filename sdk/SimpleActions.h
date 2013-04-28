#pragma once

#include "gl.h"

#include "ContextCallbacks.h"

class SimpleActions : public ContextCallbacks
{
private:
	void displayImpl(){
		glClearColor(0.5f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
};

