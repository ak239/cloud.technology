#include "GLApp.h"

#include "gl.h"

void GLApp::glutInitialize(int args, char* argv[])
{
	glutInit(&args, argv);
}

bool GLApp::glewInitialize()
{
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		lastError = reinterpret_cast<const char*>(glewGetErrorString(res));
		return false;
	}
	TwInit(TW_OPENGL, nullptr);
	TwGLUTModifiersFunc(glutGetModifiers);
	return true;
}

int GLApp::exec()
{
	glutMainLoop();
	return 0;
}
