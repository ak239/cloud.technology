#pragma once

#include <vector>
#include <ctime>
#include <boost/thread.hpp>
#include <memory>

#include "../gl.h"
#include "utils.h"
#include "../ContextCallbacks.h"
#include "../ControlledCamera.h"

#include "VolumetricCloud.h"
#include "GameScene.h"

class Clouds : public ContextCallbacks {
public:
	Clouds():
		m_gameScene(0),camera(0), numCloud(0), newNumCloud(4), 
		sunColor(1.0f, 1.0f, 1.0f, 1.0f), sunColorIntensity(1.4f), 
		windVelocity(10.0f), cellSize(12.0f), cloudEvolvingSpeed(0.8f)
	{
	}

	~Clouds(){
		delete m_gameScene;
	}

	bool hasIdleFunc() { return true; }
	typedef std::vector<CVolumetricCloud>::iterator CloudIterator;

private:

	static void RunUpdate(CloudIterator fst, CloudIterator lst);
	static bool CompareViewDistance2( CVolumetricCloud* pCloud1, CVolumetricCloud* pCloud2);
	static void TW_CALL ApplyCallback(void *clientData);

	void initImpl();
	void mouseImpl(int button, int state, int x, int y);
	void motionImpl(int x, int y);
	void passiveMotionImpl(int x, int y);
	void displayImpl();
	void keyboardImpl(unsigned char key, int x, int y);
	void reshapeImpl(int width, int height);

	void generateClouds();
	void generateScene();
	void generateTweakBars();
	void setupGL();

	void stopCalcThreads();

private:
	static bool allCalcThreadsStop;

	CGameScene* m_gameScene;
	std::vector<std::shared_ptr<boost::thread> > threads;
	TwBar* m_TwBar;

	ControlledCamera* camera;
	int numCloud;
	int newNumCloud;
	std::vector<CVolumetricCloud> clouds;
	PersProjInfo* projInfo;

	std::vector<CVolumetricCloud*> pClouds;

	glm::vec4 sunColor;
	GLfloat   sunColorIntensity;
	GLfloat   windVelocity;
	GLfloat   cellSize;
	GLfloat   cloudEvolvingSpeed;
};