#pragma once

#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>

#include "Camera.h"

class ContextCallbacks;

struct GLContextData;

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
	
	void initTweakBars();
	bool getIsInitTweakBars() const{ return isTweakBarsInit; }

	void setCamera(Camera* _camera);
	Camera* getCamera();

	static GLContext getCurrentContext();

	static const int INVALID_CONTEXT;
private:
	static void displayFunc();
	static void idleFunc();
	static void reshapeFunc(int width, int height);

	static void mouseFunc(int button, int state, int x, int y);
	static void motionFunc(int x, int y);
	static void passiveMotionFunc(int x, int y);
	static void keyboardFunc(unsigned char key, int x, int y);
	static void specialFunc(int key, int x, int y);
	static void closeFunc();

	int     windowId;
	bool    isTweakBarsInit;

	GLContextData* data;

	typedef boost::shared_ptr<ContextCallbacks> CallbackHolder;
	static boost::unordered_map<int, CallbackHolder> Callbacks;
	static boost::unordered_map<int, GLContextData*> Datas;
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
	int  oldWindowId;
	bool hasAntBar;
};


