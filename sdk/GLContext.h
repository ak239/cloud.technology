#pragma once

#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>

class ContextCallbacks;

/**
*  Store GL context
*  See GLContextGetter for get and return context
*/
class GLContext
{
public:
	GLContext(int _windowId);

	int getWindowId() const;

	void registerCallbacks(ContextCallbacks* callbacks) const;
	static GLContext getCurrentContext();

	static const int INVALID_CONTEXT;
private:
	static void displayFunc();
	static void idleFunc();

	int windowId;

	typedef boost::shared_ptr<ContextCallbacks> CallbackHolder;
	static boost::unordered_map<int, CallbackHolder> Callbacks;
};

/**
* Create GLContextGetter object for get context
* Context will be return to old after object destroy
*/
class GLContextGetter
{
public:
	GLContextGetter(const GLContext& context);
	~GLContextGetter();
private:
	int oldWindowId;
};


