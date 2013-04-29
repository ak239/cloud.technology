#include "GlutWindow.h"

#include "gl.h"

GlutWindow::GlutWindow(const std::string& name):context(glutCreateWindow(name.c_str()))
{
	GLContextGetter get(context);
}


GlutWindow::GlutWindow(int _windowId):context(_windowId)
{ 	
	GLContextGetter get(context);
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

TwBar* GlutWindow::addBar(const std::string& name)
{
	GLContextGetter get(context);
	return TwNewBar(name.c_str());
}

GlutWindow GlutWindow::Fullscreen(const std::string& mode)
{
	glutGameModeString(mode.c_str());
	return GlutWindow(glutEnterGameMode());
}
