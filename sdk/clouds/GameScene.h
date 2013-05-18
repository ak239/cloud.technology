#pragma once
#include "../gl.h"
#include "../ContextCallbacks.h"
#include "../Texture.h"
#include "../ControlledCamera.h"
#include "../FSShaderProgramLoader.h"
#include "../TransformMatrix.h"
#include "../UniformWrapper.h"
#include "../PersProjInfo.h"
#include "Skybox.h"

#include <glm\ext.hpp>

class CGameScene
{
public:
	float				m_fLength, m_fWidth, m_fHigh;// actual size of cloud in world space
	float				m_fCellSize; // size of cell in world space.
	GLint				m_uLength, m_uWidth, m_uHigh;
	glm::vec3			m_vCloudPos; // the coordinates of the left-down corner 
	glm::vec3			m_vViewpoint;
	glm::vec3			m_vSunlightDir;
	glm::vec3			m_cSunColor;
	WCHAR				m_szTextureFile[MAX_PATH];
	//LPDIRECT3DTEXTURE9	m_pTexture; // the cloud texture
	double				m_fTimeA;
	float				m_fEvolvingSpeed;
	int                 m_iSun[2];


public:
	CGameScene();
	~CGameScene();
	bool Setup(GLContext const & context);
	void onLostDevice();
	void onResetDevice();
	void onDestroyDevice();
	void Render(double fTime, float fElapsedTime);
	void Keydown(WPARAM wParam);
	void MouseMove(int xPos, int yPos, WPARAM wParam);

private:
	SkyBox* skybox;
};