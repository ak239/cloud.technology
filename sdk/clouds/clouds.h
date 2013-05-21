#pragma once

#include <vector>
#include <cstring>
#include <cstdio>

#include "../gl.h"
#include "../ContextCallbacks.h"
#include "../Texture.h"
#include "../ControlledCamera.h"
#include "../FSShaderProgramLoader.h"
#include "../TransformMatrix.h"
#include "../UniformWrapper.h"
#include "VolumetricCloud.h"
#include "utils.h"
#include "Primitives.h"

#include "GameScene.h"

#include <glm\ext.hpp>

ControlledCamera* gCamera;
PersProjInfo* gPersProjInfo;

const unsigned int gNumCloud = 16;
CVolumetricCloud g_VolumetricCloud[gNumCloud];

std::vector< CVolumetricCloud* > g_v_pClouds;


struct CloudPosSize
{
	float       x; //pos.x
	float       y; //pos.y
	float       z; //pos.z
	float       l; //length
	float       w; //width
	float       h; //height
};

struct SunColor
{
	float       r;
	float       g;
	float       b;    
};

SunColor    g_SunColor = {1.0f,1.0f,1.0f};
float       g_fSunColorIntensity = 1.4f;
float       g_fWindVelocity = 40.f;


#define CLOUD_POSY 350.f
#define CLOUD_POSY1 450.f
#define CLOUD_POSY2 300.f

float g_CellSize = 12.f;
float g_CloudEvolvingSpeed = 0.8f;

CloudPosSize g_Cloud[]={
	{-800.0f, CLOUD_POSY, 200.0f, 800.f,300.f,80.f},
	{-600.0f, CLOUD_POSY1, 50.0f, 900.f,600.f,100.f },
	{-350.0f, CLOUD_POSY, 300.0f, 400.f,200.f,120.f},
	{-150.0f, CLOUD_POSY2, 0.0f, 400.f,400.f,80.f},
	{-900.0f, CLOUD_POSY2, -400.0f, 600.f,700.f,110.f},
	{-750.0f, CLOUD_POSY, -350.0f, 600.f,200.f,130.f},
	{-300.0f, CLOUD_POSY1, -100.0f, 300.f,200.f,80.f},
	{-250.0f, CLOUD_POSY, -300.0f, 900.f,200.f,70.f},
	
	{-100.0f, CLOUD_POSY2, -250.0f, 400.f,200.f,140.f},

	{-900.0f, CLOUD_POSY1, -800.0f, 600.f,400.f,80.f},

	{-800.0f, CLOUD_POSY2, -700.0f, 400.f,200.f,90.f},
	{-700.0f, CLOUD_POSY, -680.0f, 600.f,150.f,80.f},
	{-750.0f, CLOUD_POSY1, -800.0f, 400.f,200.f,120.f},
	{-450.0f, CLOUD_POSY, -750.0f, 400.f,600.f,80.f},
	{-225.0f, CLOUD_POSY1,-700.0f, 400.f,250.f,110.f},
	{-200.0f, CLOUD_POSY, -900.0f, 400.f,100.f,80.f},
};

static bool CompareViewDistance2( CVolumetricCloud* pCloud1, CVolumetricCloud* pCloud2)
{
	return ( pCloud1->GetViewDistance() > pCloud2->GetViewDistance() );
}

class Clouds : public ContextCallbacks {
public:
	Clouds() {

	}

	~Clouds(){
		delete m_gameScene;
	}

	bool hasIdleFunc() { return true; }

private:

	void initImpl(){
		int width  = glutGet(GLUT_WINDOW_WIDTH);
		int height = glutGet(GLUT_WINDOW_HEIGHT);
		GLfloat aspect = static_cast<GLfloat>(width) / height;
		gCamera = new ControlledCamera(width, height);
		gCamera->setPos(glm::vec3(0.0f, 0.0f, 0.0f));
		gCamera->setUp(glm::vec3(0.0f, -1.0f, 0.0f));
		gCamera->setHorizontalAngle(-1.5917f);
		gCamera->setVerticalAngle(0.7166f);
		gCamera->setSpeed(100);
		gCamera->setMode(false);
		gCamera->Update();

		gPersProjInfo = new PersProjInfo();
		gPersProjInfo->FOV = 60.0f;
		gPersProjInfo->Height = (float)height;
		gPersProjInfo->Width = (float)width;
		gPersProjInfo->zNear = 1.0f;
		gPersProjInfo->zFar = 10000.0f; 

		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA);

		fprintf(
			stdout,
			"INFO: OpenGL Version: %s\n",
			glGetString(GL_VERSION)
			);

		// setup gl
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glFrontFace(GL_CW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_POINT_SPRITE);
		
		//tweakbar
		m_TwBar = TwNewBar("Menu");
		TwDefine("Menu color='0 0 0' alpha=128 position='10 10' size='200 150'");
		TwDefine("Menu fontresizable=false resizable=false");

		TwAddVarRW(m_TwBar, "sunColorIntensity", TW_TYPE_FLOAT, &g_fSunColorIntensity, "label='Sun color intensity'");
		TwAddVarRW(m_TwBar, "sunColor", TW_TYPE_COLOR3F, &g_SunColor, "label='Sun light color'");
		TwAddVarRW(m_TwBar, "cellSize", TW_TYPE_FLOAT, &g_CellSize, "label='Cell size' step=1 min=4 max=20");

		InitCloud(false);

		m_gameScene = new CGameScene();
		m_gameScene->Setup(getContext());

		for (int fTime = 0; fTime < 2; fTime ++){
			for(int i=0; i!= gNumCloud; i++ ) {
				g_VolumetricCloud[i].AdvanceTime((float)fTime, 1);
			}
		}
		
		ExitOnGLError("Init failed");

	}

	void mouseImpl(int button, int state, int x, int y) {
		gCamera->mouseClickFunc(button, state, x, y);
	}

	void motionImpl(int x, int y) {		
		gCamera->mouseMotionFunc(x,y);
	}

	void passiveMotionImpl(int x, int y) {		
		gCamera->mousePassiveMotionFunc(x,y);
	}

	void InitCloud(bool bNeedClean)
	{
		/*if (bNeedClean)
		{
		if (g_JobResult != NULL)
		{
		g_JobResult->waitUntilDone();
		}
		for (int i = 0; i < gNumCloud; i++)
		g_VolumetricCloud[i].Cleanup();
		}*/
		Environment Env;
		Env.cSunColor = glm::vec4( g_SunColor.r, g_SunColor.g, g_SunColor.b, 1.0f );
		Env.fSunColorIntensity = g_fSunColorIntensity;
		Env.vWindVelocity = glm::vec3( -0.f, 0.f, -g_fWindVelocity );
		Env.vSunlightDir = glm::vec3( 1.0f, -1.0f, 0.0f );

		CloudProperties Cloud;
		Cloud.fCellSize = g_CellSize;//1.5;
		Cloud.fEvolvingSpeed = (float)(1.0-g_CloudEvolvingSpeed);

		sprintf_s(Cloud.szTextureFile,MAX_PATH, "%s", "metaball.dds");

		g_v_pClouds.clear();
		for (int i = 0; i < gNumCloud; i++)
		{
			Cloud.fLength = g_Cloud[i].l;
			Cloud.fWidth  = g_Cloud[i].w;
			Cloud.fHigh   = g_Cloud[i].h;

			Cloud.vCloudPos = glm::vec3( g_Cloud[i].x, g_Cloud[i].y,g_Cloud[i].z);
			g_VolumetricCloud[i].Setup( getContext(), &Env, &Cloud );
			g_v_pClouds.push_back(&g_VolumetricCloud[i]);
		}
	}


	void displayImpl() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);

		m_gameScene->Render(0.0,0.0f);

		for ( int i=0; i<gNumCloud; ++i )
		{
			g_VolumetricCloud[i].UpdateViewDistance();
		}

		std::sort(g_v_pClouds.begin(), g_v_pClouds.end(), &CompareViewDistance2);

		std::vector< CVolumetricCloud* >::iterator itCurCP, itEndCP = g_v_pClouds.end();
		for( itCurCP = g_v_pClouds.begin(); itCurCP != itEndCP; ++ itCurCP )	
		{
			(*itCurCP)->Render();
		}

	}

	void keyboardImpl(unsigned char key, int x, int y) {
		switch ( key )
		{
			case 27: 
				glutDestroyWindow ( getContext().getWindowId() );
				exit (0);
				break;
		}
		gCamera->keyboardFunc(key, x, y);
	}

	void idle() {
		

		gCamera->idleFunc();
	}


	void reshapeImpl(int width, int height) {
		glViewport(0, 0, width, height);
		gCamera->reshapeFunc(width, height);
		gPersProjInfo->Height = (float)height;
		gPersProjInfo->Width = (float)width;
		ExitOnGLError("Reshape failed");
	}


private:
	CGameScene* m_gameScene;
	TwBar* m_TwBar;
};