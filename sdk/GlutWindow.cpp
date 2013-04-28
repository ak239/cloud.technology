#include "GlutWindow.h"

#include "gl.h"

GlutWindow::GlutWindow(const std::string& name):context(glutCreateWindow(name.c_str()))
{
}


GlutWindow::GlutWindow(int _windowId):context(_windowId)
{ 	
}

GlutWindow::~GlutWindow(void)
{
	glutDestroyWindow(context.getWindowId());
}

void GlutWindow::setPosition(int x, int y)
{
	GLContextGetter get(context);
	glutPositionWindow(x, y);
}


void GlutWindow::setGeometry(int width, int height)
{
	GLContextGetter get(context);
	glutReshapeWindow(width, height);
}

GlutWindow GlutWindow::subWindow(int x, int y, int width, int height)
{
	GLContextGetter get(context);
	int subWindowId = glutCreateSubWindow(context.getWindowId(), x, y, width, height);
	return GlutWindow(subWindowId);
}
