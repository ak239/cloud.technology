#pragma once

#include "..\gl.h"
#include "..\GLContext.h"
#include "Simulation.h"
#include "CloudShader.h"
#include "CloudParticle.h"

#define SAMPLE_LENGTH			0.5f 
#define OPTICAL_LENGTH_FACTOR	1.5f // determine the white and black contrast.more big more contrast.
#define SCATTER_FACTOR			0.6f // According to the scattering model used, to have the intensity of scattering color less than the incident color,
// the maximum of SCATTER_FACTOR should be 2/3.
#define MIN_DENSITY				0.05f


struct Environment
{
	glm::vec3			vWindVelocity;
	glm::vec3			vSunlightDir;
	glm::vec4			cSunColor;
	float               fSunColorIntensity;
};

struct CloudProperties
{
	float	    fLength;
	float	    fWidth;
	float	    fHigh;
	float	    fCellSize;
	float	    fEvolvingSpeed;
	glm::vec3   vCloudPos;
	std::string textureFile;
};


class CVolumetricCloud
{
public:
	float				m_fLength, m_fWidth, m_fHigh;// actual size of cloud in world space
	float				m_fCellSize; // size of cell in world space.
	int 				m_iLength, m_iWidth, m_iHigh;
	glm::vec3			m_vCloudPos; // the coordinates of the left-down corner 
	glm::vec3			m_vWindVelocity;
	glm::vec3			m_vViewpoint;
	glm::vec3			m_vSunlightDir;
	glm::vec4			m_cSunColor;
	char				m_szTextureFile[MAX_PATH];
	//LPDIRECT3DTEXTURE9	m_pTexture; // the cloud texture
	double				m_fTimeA;
	float				m_fEvolvingSpeed;
	//LPDIRECT3DDEVICE9	m_pDevice;
	CParticlePool		m_ParticlePool;
	float               m_fSunColorIntensity;
	int                 m_iColorUpdateInterval[2];

	float			 	m_fViewDistances;
	double              m_fTime;

	glm::vec3           m_lastTarget;

protected:
	CSimulationSpace	m_Simulator;
	CCloudShader		m_CloudShader;

public:
	CVolumetricCloud();
	~CVolumetricCloud();
	bool Setup(GLContext const & context, Environment *Env, CloudProperties *Cloud );
	void Cleanup();
	void AdvanceTime(double fTime, int interval);
	void Render();

	bool GenerateCloudParticles();
	void CleanupCloudParticles();
	void UpdateCloudParticleColors();
	void SortCloudParticles(const glm::vec3 &vLookDir);
	void UpdateCloudPosition(double fTime);

	void SetEvolvingSpeed(float speed);
	void SetSunColor(float r, float g, float b,float i);
	void SetSunColorIntensity(float i);
	void SetWindVelocity(float v);

	float	GetViewDistance(){return m_fViewDistances;};
	void	UpdateViewDistance();

protected:
	glm::vec4	CalculateParticleIncidentColor( CloudParticle* pCloudParticle );
	glm::vec4	CalculateParticleIncidentColor_SSE( CloudParticle* pCloudParticle );
	glm::vec4	CalculateParticleScatteringColor( CloudParticle* pCloudParticle );
};
