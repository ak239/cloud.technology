#pragma once

#include <string>
#include <boost/unordered_map.hpp>
#include "ContextCallbacks.h"
#include "GLContext.h"

class GlutWindow
{
public:
	GlutWindow(const std::string& name);
	~GlutWindow(void);

	GlutWindow subWindow(int x, int y, int width, int height);

	void setPosition(int x, int y);
	void setGeometry(int width, int height);

	const GLContext& getContext() const{ return context; }

private:
	GlutWindow(int _windowId);

	const GLContext context;
};


