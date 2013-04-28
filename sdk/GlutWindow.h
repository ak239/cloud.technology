#pragma once

#include <string>
#include <boost/unordered_map.hpp>
#include "ContextCallbacks.h"
#include "GLContext.h"
#include <AntTweakBar.h>

class GlutWindow
{
public:
	GlutWindow(const std::string& name);
	~GlutWindow(void);

	GlutWindow subWindow(int x, int y, int width, int height);

	void setPosition(int x, int y);
	void setGeometry(int width, int height);

	TwBar* addBar(const std::string& name);

	const GLContext& getContext() const{ return context; }
	GLContext& getContext(){ return context; }

private:
	GlutWindow(int _windowId);

	std::string barName;

	GLContext context;
};


