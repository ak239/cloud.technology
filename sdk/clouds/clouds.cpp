#include "clouds.h"

struct CloudPosSize
{
	float       x; //pos.x
	float       y; //pos.y
	float       z; //pos.z
	float       l; //length
	float       w; //width
	float       h; //height
};

struct UpdateCloud {
    void operator()( Clouds::CloudIterator fst, Clouds::CloudIterator lst ) const {
		const int colorUpdateInterval = 1;
		double fTime = (double)clock() / CLOCKS_PER_SEC;
        for(Clouds::CloudIterator it = fst; it != lst; ++it ) {
                it->AdvanceTime(fTime , colorUpdateInterval);
        }
    }
};

bool Clouds::allCalcThreadsStop = false;

namespace
{
	const float CLOUD_POSY = 350.f;
	const float CLOUD_POSY1 = 450.f;
	const float CLOUD_POSY2 = 300.f;

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
}

void Clouds::RunUpdate(CloudIterator fst, CloudIterator lst)
{
	UpdateCloud updater;
	while (!Clouds::allCalcThreadsStop){
		updater(fst, lst);
	}
}

bool Clouds::CompareViewDistance2( CVolumetricCloud* pCloud1, CVolumetricCloud* pCloud2)
{
	return ( pCloud1->GetViewDistance() > pCloud2->GetViewDistance() );
}

void TW_CALL Clouds::ApplyCallback(void *clientData)
{ 
	static_cast<Clouds*>(clientData)->generateClouds();
	static_cast<Clouds*>(clientData)->generateScene();
}

void Clouds::initImpl(){
	setupGL();

	GLContext current = GLContext::getCurrentContext();

	int width  = current.getWidth();
	int height = current.getHeight();

	camera = new ControlledCamera(width, height);
	camera->Update();

	current.setCamera(camera);

	projInfo = new PersProjInfo(60.0f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 1.0f, 10000.0f);
	current.setPersProjInfo(projInfo);

	generateClouds();
	generateScene();
	generateTweakBars();
		
	ExitOnGLError("Init failed");
}

void Clouds::mouseImpl(int button, int state, int x, int y) {
	camera->mouseClickFunc(button, state, x, y);
}

void Clouds::motionImpl(int x, int y) {		
	camera->mouseMotionFunc(x,y);
}

void Clouds::passiveMotionImpl(int x, int y) {		
	camera->mousePassiveMotionFunc(x,y);
}

void Clouds::displayImpl() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);

	m_gameScene->Render(0.0, 0.0f);

	for (std::size_t i=0; i< clouds.size(); ++i )
		clouds[i].UpdateViewDistance();

	std::sort(pClouds.begin(), pClouds.end(), &CompareViewDistance2);

	std::vector< CVolumetricCloud* >::iterator itCurCP, itEndCP = pClouds.end();
	for( itCurCP = pClouds.begin(); itCurCP != itEndCP; ++ itCurCP )	
		(*itCurCP)->Render();

	ExitOnGLError("Display failed");
}

void Clouds::keyboardImpl(unsigned char key, int x, int y) {
	switch ( key )
	{
		case 27: 
			stopCalcThreads();
			glutDestroyWindow ( getContext().getWindowId() );
			exit (0);
			break;
	}
	camera->keyboardFunc(key, x, y);
}

void Clouds::reshapeImpl(int width, int height) {
	glViewport(0, 0, width, height);
		
	camera->reshapeFunc(width, height);

	projInfo->Height = (GLfloat)height;
	projInfo->Width  = (GLfloat)width;

	ExitOnGLError("Reshape failed");
}

void Clouds::generateClouds()
{
	stopCalcThreads();

	threads.swap(std::vector<std::shared_ptr<std::thread> >());

	Environment Env;
	Env.cSunColor          = sunColor;
	Env.fSunColorIntensity = sunColorIntensity;
	Env.vWindVelocity      = glm::vec3( -0.f, 0.f, -windVelocity);
	Env.vSunlightDir       = glm::vec3( 1.0f, -1.0f, 0.0f );

	CloudProperties Cloud;
	Cloud.fCellSize      = cellSize;
	Cloud.fEvolvingSpeed = 1.0f - cloudEvolvingSpeed;
	Cloud.textureFile    = ".\\media\\metaball.dds";

	pClouds.clear();
	clouds.swap(std::vector<CVolumetricCloud>(newNumCloud));
	numCloud = newNumCloud;
	for (int i = 0; i < numCloud; i++)
	{
		Cloud.fLength = g_Cloud[i].l;
		Cloud.fWidth  = g_Cloud[i].w;
		Cloud.fHigh   = g_Cloud[i].h;

		Cloud.vCloudPos = glm::vec3( g_Cloud[i].x, g_Cloud[i].y,g_Cloud[i].z);
		clouds[i].Setup( getContext(), &Env, &Cloud );
		pClouds.push_back(&clouds[i]);
	}

	// run calc threads
	Clouds::allCalcThreadsStop = false;
	const int THREAD_COUNT = 2;

	int per_thread = numCloud / THREAD_COUNT;
	for (int i = 0; i < THREAD_COUNT; ++i){
		int lst = (i + 1) * per_thread;

		CloudIterator fstIt = clouds.begin();
		std::advance(fstIt, i * per_thread);

		CloudIterator lstIt = clouds.begin();
		std::advance(lstIt, lst > numCloud ? numCloud : lst); 

		threads.push_back(std::shared_ptr<std::thread>(new std::thread(&RunUpdate, fstIt, lstIt)));			
	}
}

void Clouds::generateScene()
{
	delete m_gameScene;
	m_gameScene = new CGameScene();
	m_gameScene->Setup(getContext());
}

void Clouds::generateTweakBars()
{
	m_TwBar = TwNewBar("Menu");
	TwDefine("Menu color='0 0 0' alpha=128 position='10 10' size='200 250'");
	TwDefine("Menu fontresizable=false resizable=false");

	TwAddVarRW( m_TwBar, "sunColorIntensity",  TW_TYPE_FLOAT,   &sunColorIntensity,  "label='Sun color intensity'");
	TwAddVarRW( m_TwBar, "sunColor",           TW_TYPE_COLOR3F, &sunColor[0],		 "label='Sun light color'");
	TwAddVarRW( m_TwBar, "cellSize",           TW_TYPE_FLOAT,   &cellSize,			 "label='Cell size'   step=1 min=4 max=20");
	TwAddVarRW( m_TwBar, "cloudCount",         TW_TYPE_INT16,   &newNumCloud,		 "label='Cloud count' step=1 min=4 max=16");
	TwAddVarRW( m_TwBar, "windVelocity",       TW_TYPE_FLOAT,   &windVelocity,		 "label='Wind velocity'");
	TwAddVarRW( m_TwBar, "cloudEvolvingSpeed", TW_TYPE_FLOAT,   &cloudEvolvingSpeed, "label='Cloud Evolving Speed' step=0.1 max=1.0 min=0.1");
	TwAddButton(m_TwBar, "Apply", &Clouds::ApplyCallback, this, "");
}

void Clouds::setupGL(){
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SPRITE);
}

void Clouds::stopCalcThreads()
{
	Clouds::allCalcThreadsStop = true;

	for (std::size_t i = 0; i < threads.size(); ++i)
		threads[i]->join();

	threads.clear();
}
