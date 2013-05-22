#include "GameScene.h"

CGameScene::CGameScene()
{
}

CGameScene::~CGameScene()
{
}

bool CGameScene::Setup(const GLContext & context)
{
	skybox = new SkyBox(context, context.getCamera(), *context.getPersProjInfo());	
	skybox->Init(".\\sdk\\media\\",
		"posx.dds",
		"negx.dds",
		"posy.dds",
		"negy.dds",
		"posz.dds",
		"negz.dds"
		);
	return true;
}

void CGameScene::Render(double fTime, float fElapsedTime) {
	skybox->Render();
}