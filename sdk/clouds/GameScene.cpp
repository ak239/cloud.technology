#include "GameScene.h"

extern PersProjInfo* gPersProjInfo;

CGameScene::CGameScene()
{
}

CGameScene::~CGameScene()
{
}

bool CGameScene::Setup(const GLContext & context)
{
	skybox = new SkyBox(context, context.getCamera(), *gPersProjInfo);	
	skybox->Init(".",
		"posx.bmp",
		"negx.bmp",
		"posy.bmp",
		"negy.bmp",
		"posz.bmp",
		"negz.bmp"
		);
	return true;
}

void CGameScene::Render(double fTime, float fElapsedTime) {
	skybox->Render();
}