#include "GameScene.h"


extern ControlledCamera* gCamera;
extern PersProjInfo* gPersProjInfo;

CGameScene::CGameScene()
{
}

CGameScene::~CGameScene()
{
}

bool CGameScene::Setup(const GLContext & context)
{
	skybox = new SkyBox(context, gCamera, *gPersProjInfo);	
	skybox->Init(".",
		"posx.bmp",
		"negx.bmp",
		"posy.bmp",
		"negy.bmp",
		"posz.bmp",
		"negz.bmp"
		);
	/*skybox->Init(".",
		"green.bmp",
		"red.bmp",
		"yellow.bmp",
		"blue.bmp",
		"purple.bmp",
		"orange.bmp"
		);*/
	return true;
}

void CGameScene::Render(double fTime, float fElapsedTime) {
	skybox->Render();
}