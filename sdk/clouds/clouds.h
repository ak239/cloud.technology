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
#include <ctime>

#include <glm\ext.hpp>
#include <thread>
#include <memory>
#include "../nvModel.h"

ControlledCamera* gCamera;

int gNumCloud = 4;
int g_NewCloudNum = 4;
std::vector<CVolumetricCloud> g_VolumetricCloud;
PersProjInfo*      gPersProjInfo;

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
float       g_fWindVelocity = 10.f;


const float CLOUD_POSY = 350.f;
const float CLOUD_POSY1 = 450.f;
const float CLOUD_POSY2 = 300.f;

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

//vertex buffer objects and texture buffers used for rendering
GLuint textures[2];
GLuint textureBuffers[2];
GLuint vbo;
GLuint indexBuffer[2];

static bool CompareViewDistance2( CVolumetricCloud* pCloud1, CVolumetricCloud* pCloud2)
{
	return ( pCloud1->GetViewDistance() > pCloud2->GetViewDistance() );
}

int g_iColorUpdateInterval = 1;
const float TIME_STEP = 0.5f;

struct UpdateCloud {
    void operator()( int fst, int lst ) const {
		double fTime = (double)clock() / CLOCKS_PER_SEC;
//#pragma omp parallel for
        for(int i=fst; i < lst; i++ ) {
                g_VolumetricCloud[i].AdvanceTime(fTime , g_iColorUpdateInterval);
        }
    }
     UpdateCloud(double fTime = 0.0)
     {}
};

bool allCalcThreadsStop;
const int THREAD_COUNT = 2;
UpdateCloud g_pUpdateCloud;

static void RunUpdate(int fst, int lst)
{
	while (!allCalcThreadsStop){
		g_pUpdateCloud(fst, lst);
	}
}


class Clouds : public ContextCallbacks {
public:
	Clouds():m_gameScene(0) {

	}

	~Clouds(){
		delete m_gameScene;
	}

	bool hasIdleFunc() { return true; }

private:

	static void TW_CALL ApplyCallback(void *clientData)
	{ 
		static_cast<Clouds*>(clientData)->generateClouds();
		static_cast<Clouds*>(clientData)->generateScene();
	}

	void initImpl(){
		setupGL();

		//nv::Model model;
		//if (!model.loadModelFromFile("D:\\Develop\\nir\\sdk\\sdk\\Su-37_Terminator\\Su-37_Terminator.obj"))
		//	fprintf(stderr, "Obj load error!\r\n");


		//glGenTextures(2, textures);
		//glGenBuffers( 2, textureBuffers);
		//glGenBuffers( 2, indexBuffer);
		//glGenBuffers( 1, &vbo);

		//compute the model dimensions
		//nv::vec3f minPos, maxPos;

		//model->computeBoundingBox( minPos, maxPos);
		//center = (minPos + maxPos) * 0.5f;
		//diameter = nv::length( maxPos - minPos);

		//manipulator.setDollyPosition( -1.5f * diameter);

		//create and fill the texture buffers

		//vertex texture buffer first
		//glBindBuffer( GL_TEXTURE_BUFFER_EXT, textureBuffers[0]);

		// must pad positions to size 4 for texbo efficiency
		//if ( model->getPositionSize() == 3) {
  //      
		//	float *tempPos = new float[model->getPositionCount() * 4];
		//	const float *src = model->getPositions();
		//	float *dst = tempPos;
		//	for (int ii = 0; ii<model->getPositionCount(); ii++) {
		//		*dst++ = *src++;
		//		*dst++ = *src++;
		//		*dst++ = *src++;
		//		*dst++ = 1.0f;
		//	}
		//	glBufferData( GL_TEXTURE_BUFFER_EXT, 4 * model->getPositionCount() * sizeof(float), tempPos, GL_STATIC_DRAW);
		//	delete []tempPos;
		//}
		//else {
		//	glBufferData( GL_TEXTURE_BUFFER_EXT, model->getPositionSize() * model->getPositionCount() * sizeof(float), model->getPositions(), GL_STATIC_DRAW);
		//}
		//glBindTexture( GL_TEXTURE_BUFFER_EXT, textures[0]);
		//glTexBufferEXT( GL_TEXTURE_BUFFER_EXT, GL_RGBA32F_ARB, textureBuffers[0]);

		//normal texture buffer first
		//glBindBuffer( GL_TEXTURE_BUFFER_EXT, textureBuffers[1]);
		// must pad normals to size 4 for texbo efficiency
		//if ( model->getNormalSize() == 3) {
		//	float *tempNorm = new float[model->getNormalCount() * 4];
		//	const float *src = model->getNormals();
		//	float *dst = tempNorm;
		//	for (int ii = 0; ii<model->getNormalCount(); ii++) {
		//		*dst++ = *src++;
		//		*dst++ = *src++;
		//		*dst++ = *src++;
		//		*dst++ = 1.0f;
		//	}
		//	glBufferData( GL_TEXTURE_BUFFER_EXT, 4 * model->getNormalCount() * sizeof(float), tempNorm, GL_STATIC_DRAW);
		//	delete []tempNorm;
		//}
		//else {
		//	glBufferData( GL_TEXTURE_BUFFER_EXT, model->getNormalSize() * model->getNormalCount() * sizeof(float), model->getNormals(), GL_STATIC_DRAW);
		//}
		//glBindTexture( GL_TEXTURE_BUFFER_EXT, textures[1]);
		//glTexBufferEXT( GL_TEXTURE_BUFFER_EXT, GL_RGBA32F_ARB, textureBuffers[1]);

		////create the index list
		//float *indices = new float[ model.getIndexCount() * 2];

		//const GLuint *vtxIndex = model.getPositionIndices();
		//const GLuint *nrmIndex = model.getNormalIndices();
		//float *wlk = indices;

		//for (int ii=0; ii<model.getIndexCount(); ii++) {
		//	*wlk++ = (float)*vtxIndex++;
		//	*wlk++ = (float)*nrmIndex++;
		//}

		//glBindBuffer( GL_ARRAY_BUFFER, indexBuffer[0]);
		//glBufferData( GL_ARRAY_BUFFER, sizeof(GLuint) * model.getIndexCount() * 2, indices, GL_STATIC_DRAW);

		//delete []indices;

		////now, compute a compiled model or reference
		//model.compileModel();

		//// load the vertex data for the compiled model
		//glBindBuffer( GL_ARRAY_BUFFER, vbo);
		//glBufferData( GL_ARRAY_BUFFER, model.getCompiledVertexSize() * model.getCompiledVertexCount() * sizeof(float), model.getCompiledVertices(), GL_STATIC_DRAW);

		////load the index data for the compiled model
		//glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer[1]);
		//glBufferData( GL_ELEMENT_ARRAY_BUFFER, model.getCompiledIndexCount() * sizeof(GLuint), model.getCompiledIndices(), GL_STATIC_DRAW);

		//glBindBuffer( GL_ARRAY_BUFFER, 0);
		//glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0);
		//glBindBuffer( GL_TEXTURE_BUFFER_EXT, 0);

		
		int width  = glutGet(GLUT_WINDOW_WIDTH);
		int height = glutGet(GLUT_WINDOW_HEIGHT);

		gCamera = new ControlledCamera(width, height);
		gCamera->Update();

		GLContext::getCurrentContext().setCamera(gCamera);

		gPersProjInfo = new PersProjInfo(60.0f, static_cast<float>(width), static_cast<float>(height), 1.0f, 10000.0f);
		
		//tweakbar
		m_TwBar = TwNewBar("Menu");
		TwDefine("Menu color='0 0 0' alpha=128 position='10 10' size='200 150'");
		TwDefine("Menu fontresizable=false resizable=false");

		TwAddVarRW( m_TwBar, "sunColorIntensity", TW_TYPE_FLOAT,   &g_fSunColorIntensity, "label='Sun color intensity'");
		TwAddVarRW( m_TwBar, "sunColor",          TW_TYPE_COLOR3F, &g_SunColor,    "label='Sun light color'");
		TwAddVarRW( m_TwBar, "cellSize",          TW_TYPE_FLOAT,   &g_CellSize,    "label='Cell size'   step=1 min=4 max=20");
		TwAddVarRW( m_TwBar, "cloudCount",        TW_TYPE_INT16,   &g_NewCloudNum, "label='Cloud count' step=1 min=4 max=16");
		TwAddButton(m_TwBar, "Apply", &Clouds::ApplyCallback, this, "");

		generateClouds();
		generateScene();
		
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

	void generateClouds()
	{
		allCalcThreadsStop = true;
		for (std::size_t i = 0; i < threads.size(); ++i){
			threads[i]->join();
		}

		threads.swap(std::vector<std::shared_ptr<std::thread> >());

		Environment Env;
		Env.cSunColor          = glm::vec4( g_SunColor.r, g_SunColor.g, g_SunColor.b, 1.0f );
		Env.fSunColorIntensity = g_fSunColorIntensity;
		Env.vWindVelocity      = glm::vec3( -0.f, 0.f, -g_fWindVelocity );
		Env.vSunlightDir       = glm::vec3( 1.0f, -1.0f, 0.0f );

		CloudProperties Cloud;
		Cloud.fCellSize      = g_CellSize;//1.5;
		Cloud.fEvolvingSpeed = (float)(1.0-g_CloudEvolvingSpeed);

		sprintf_s(Cloud.szTextureFile,MAX_PATH, "%s", "metaball.dds");

		g_v_pClouds.clear();
		g_VolumetricCloud.swap(std::vector<CVolumetricCloud>(g_NewCloudNum));
		gNumCloud = g_NewCloudNum;
		for (int i = 0; i < gNumCloud; i++)
		{
			Cloud.fLength = g_Cloud[i].l;
			Cloud.fWidth  = g_Cloud[i].w;
			Cloud.fHigh   = g_Cloud[i].h;

			Cloud.vCloudPos = glm::vec3( g_Cloud[i].x, g_Cloud[i].y,g_Cloud[i].z);
			g_VolumetricCloud[i].Setup( getContext(), &Env, &Cloud );
			g_v_pClouds.push_back(&g_VolumetricCloud[i]);
		}

		// run calc threads
		allCalcThreadsStop = false;
		int per_thread = gNumCloud / THREAD_COUNT;
		for (int i = 0; i < THREAD_COUNT; ++i){
			int lst = (i + 1) * per_thread;
			threads.push_back(std::shared_ptr<std::thread>(new std::thread(&RunUpdate, i * per_thread, lst > gNumCloud ? gNumCloud : lst)));			
		}
	}

	void generateScene()
	{
		delete m_gameScene;
		m_gameScene = new CGameScene();
		m_gameScene->Setup(getContext());
	}


	void displayImpl() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);

		m_gameScene->Render(0.0, 0.0f);

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
				allCalcThreadsStop = true;

				for (std::size_t i = 0; i < threads.size(); ++i)
					threads[i]->join();

				threads.clear();

				glutDestroyWindow ( getContext().getWindowId() );

				exit (0);
				break;
		}
		gCamera->keyboardFunc(key, x, y);
	}

	void reshapeImpl(int width, int height) {
		glViewport(0, 0, width, height);
		gCamera->reshapeFunc(width, height);

		gPersProjInfo->Height = (float)height;
		gPersProjInfo->Width = (float)width;
		ExitOnGLError("Reshape failed");
	}

	void setupGL(){
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glFrontFace(GL_CW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_POINT_SPRITE);
	}

private:
	CGameScene* m_gameScene;
	std::vector<std::shared_ptr<std::thread> > threads;
	TwBar* m_TwBar;
};