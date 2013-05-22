//-------------------------------------------------------------------------------------
//
// Copyright 2009 Intel Corporation
// All Rights Reserved
//
// Permission is granted to use, copy, distribute and prepare derivative works of this
// software for any purpose and without fee, provided, that the above copyright notice
// and this statement appear in all copies.  Intel makes no representations about the
// suitability of this software for any purpose.  THIS SOFTWARE IS PROVIDED "AS IS."
// INTEL SPECIFICALLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, AND ALL LIABILITY,
// INCLUDING CONSEQUENTIAL AND OTHER INDIRECT DAMAGES, FOR THE USE OF THIS SOFTWARE,
// INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PROPRIETARY RIGHTS, AND INCLUDING THE
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  Intel does not
// assume any responsibility for any errors which may appear in this software nor any
// responsibility to update it.
//
//--------------------------------------------------------------------------------------
// DXUT was adapted from the Microsoft DirectX SDK(November 2008)
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// The skybox is free downloaded from :
//   http://en.pudn.com/downloads119/sourcecode/others/detail508412_en.html
//-------------------------------------------------------------------------------------

#include "../gl.h"
#include "VolumetricCloud.h"
#include "Simulation.h"
#include "../ControlledCamera.h"
#include "../GLContext.h"

using namespace std;

const int iMaxColorInterval = 1000;

static inline float OpticalLength(float fLineDensity)
{
	return OPTICAL_LENGTH_FACTOR * fLineDensity;
}

static float Phase( glm::vec3 vIn, glm::vec3 vOut )
{
	vIn = glm::normalize(vIn);
	vOut = glm::normalize(vOut);	
	return (float)(( 1.0 + SQUARE( glm::dot(vIn, vOut) ) )* 3.0/4.0);
}

CVolumetricCloud::CVolumetricCloud():
	m_fLength( 0.f ),
	m_fWidth( 0.f ),
	m_fHigh( 0.f ),
	m_fCellSize( 0.f ),
	m_iLength( 0 ),
	m_iWidth( 0 ),
	m_iHigh( 0 ),
	m_fTimeA(0)
{
	//for double buffer implementation
	m_iColorUpdateInterval[0] = 0;
	m_iColorUpdateInterval[1] = 1; 
}

bool CVolumetricCloud::Setup( GLContext const & context, Environment *Env, CloudProperties *Cloud )
{
	m_fLength = Cloud->fLength;
	m_fWidth = Cloud->fWidth;
	m_fHigh = Cloud->fHigh;
	m_fCellSize = Cloud->fCellSize;
	m_iLength = (int)( m_fLength/m_fCellSize+0.5 );
	m_iWidth = (int)( m_fWidth/m_fCellSize+0.5 );
	m_iHigh = (int)( m_fHigh/m_fCellSize+0.5 );
	m_fEvolvingSpeed = Cloud->fEvolvingSpeed;
	m_vCloudPos = Cloud->vCloudPos;
	// create texture;
	sprintf_s(m_szTextureFile, MAX_PATH, "%s", Cloud->textureFile.c_str());

	m_vWindVelocity = Env->vWindVelocity;

	m_fSunColorIntensity = Env->fSunColorIntensity;
	m_cSunColor.r = Env->cSunColor.r*m_fSunColorIntensity;
	m_cSunColor.g = Env->cSunColor.g*m_fSunColorIntensity;
	m_cSunColor.b = Env->cSunColor.b*m_fSunColorIntensity;

	m_vSunlightDir = Env->vSunlightDir;
	m_vSunlightDir = glm::normalize(m_vSunlightDir);

	//m_vViewpoint = *g_pvViewpoint;
	Camera* camera = GLContext::getCurrentContext().getCamera();
	m_vViewpoint = camera->getPos();

	m_Simulator.Setup( m_iLength, m_iWidth, m_iHigh );
	GenerateCloudParticles();
	m_CloudShader.Setup(context, this, (float)1.2*m_fCellSize, 200 );

	m_ParticlePool.m_vWindVelocity = Env->vWindVelocity;


	return true;
}


void CVolumetricCloud::Cleanup()
{
	m_CloudShader.Cleanup();

	m_ParticlePool.Cleanup();

	m_Simulator.Cleanup();

	/*if( m_pDevice )
	{
	m_pDevice->Release();
	m_pDevice = NULL;
	}*/
}


CVolumetricCloud::~CVolumetricCloud()
{
}


glm::vec4 CVolumetricCloud::CalculateParticleIncidentColor( CloudParticle* pCloudParticle )
{
	float fDensity;
	float fOpticalDepth;
	float fTransparency;

	glm::vec3 vCurPt = glm::vec3((float)pCloudParticle->m_i,(float)pCloudParticle->m_j,(float)pCloudParticle->m_k) 
		+ ( - m_vSunlightDir ) * SAMPLE_LENGTH;

	glm::vec4 cIncidentColor = m_cSunColor;
	float fAlpha = 1.5 * SCATTER_FACTOR;
	while( m_Simulator.IsPointInSpace(&vCurPt) )
	{
		fDensity = m_Simulator.GetPointDensity( &vCurPt );
		fOpticalDepth = OpticalLength( fDensity * SAMPLE_LENGTH ); 		
		fTransparency = (float)exp( - fOpticalDepth );
		cIncidentColor *= ( ( 1 - fAlpha ) * fTransparency + fAlpha );
		vCurPt += ( - m_vSunlightDir ) * SAMPLE_LENGTH;
	}

	return cIncidentColor;
}

glm::vec4 CVolumetricCloud::CalculateParticleScatteringColor( CloudParticle* pCloudParticle )
{
	UINT i = pCloudParticle->m_i;
	UINT j = pCloudParticle->m_j;
	UINT k = pCloudParticle->m_k;
	glm::vec4 cScatteringColor;

	float fDensity = m_Simulator.GetCellDensity( i,j,k );

	if( fDensity < MIN_DENSITY )
	{
		pCloudParticle->m_bVisible = false;
		cScatteringColor = glm::vec4(0,0,0,0);
		pCloudParticle->m_cScatteringColor = cScatteringColor;
		return cScatteringColor;
	}
	else
	{
		pCloudParticle->m_bVisible = true;

		glm::vec4 cIncidentColor = CalculateParticleIncidentColor( pCloudParticle );;

		float fOpticalDepth = OpticalLength( fDensity );
		float fTransparency = (float)exp( - fOpticalDepth );

		glm::vec3 vViewDir = *(pCloudParticle->GetPosition()) - m_vViewpoint;
		cScatteringColor = cIncidentColor
			* ( (float)(( 1 - fTransparency ) * Phase( m_vSunlightDir, vViewDir) * SCATTER_FACTOR) );

		cScatteringColor.a = 1 - fTransparency; //particle's alpha value = 1 - m_fAttenuation

		pCloudParticle->m_cScatteringColor = cScatteringColor;

		return cScatteringColor;
	}
}


bool CVolumetricCloud::GenerateCloudParticles()
{
	bool ret;
	ret = m_ParticlePool.Setup( this, m_Simulator.GetNumCellInVolume() );
	if( !ret ) return false;
	for( int i = 0; i < m_iLength; i++ )
	{
		for( int j = 0; j < m_iHigh; j++)
		{
			for( int k = 0; k < m_iWidth; k++ )
			{
				if( m_Simulator.IsCellInVolume(i,j,k) )
				{
					ret = m_ParticlePool.AddParticle(i,j,k);
					if( !ret ) return false;
				}
			}
		}
	}
	return true;
}

void CVolumetricCloud::UpdateCloudParticleColors()
{
	CParticleEnumerator Enumerator( &m_ParticlePool );
	CloudParticle *pCurParticle = Enumerator.NextParticle();
	while( pCurParticle )
	{
		CalculateParticleScatteringColor( pCurParticle );
		pCurParticle = Enumerator.NextParticle();
	}
}

void CVolumetricCloud::SortCloudParticles(const glm::vec3 &vLookDir)
{
	m_ParticlePool.SortbyViewDistances(vLookDir);
}


void CVolumetricCloud::UpdateCloudPosition(double fTime)
{
	if (m_fTime == 0.0)  m_fTime = fTime; //first frame

	glm::vec3 vDisplacement = m_vWindVelocity * (float)(fTime-m_fTime);
	m_vCloudPos += vDisplacement;
	m_ParticlePool.UpdateParticlePositions( fTime );

	//Change the cloud pos to let it loop moving
	if (m_vCloudPos.z < -1000) m_vCloudPos.z += 1500;

	m_fTime = fTime;
}

void CVolumetricCloud::Render()
{
	m_CloudShader.Render();
}


void CVolumetricCloud::AdvanceTime(double fTime, int interval)
{
	//Double buffer: switch buffer index between 0 and 1. 
	m_ParticlePool.m_iCurrentBuffer = 1 - m_ParticlePool.m_iCurrentBuffer;

	Camera* camera = GLContext::getCurrentContext().getCamera();
	m_vViewpoint = camera->getPos();

	// change cloud density
	if (m_fEvolvingSpeed != 1.0) // if not pause evolving
	{
		float fAlpha = (float)( fTime - m_fTimeA ) / m_fEvolvingSpeed;
		m_Simulator.InterpolateDensitySpace( fAlpha);
		if( ( fTime < m_fTimeA )|| ( fTime > (m_fTimeA + m_fEvolvingSpeed)))
		{
			m_fTimeA = fTime;
		}
	}

	UpdateCloudPosition( fTime );
	if (camera->getTarget() != m_lastTarget){
		SortCloudParticles(camera->getTarget());    
		m_lastTarget = camera->getTarget();
	}

	if ((m_iColorUpdateInterval[m_ParticlePool.m_iCurrentBuffer]%interval) == 0)
		UpdateCloudParticleColors();
	m_iColorUpdateInterval[m_ParticlePool.m_iCurrentBuffer]++;
	if (m_iColorUpdateInterval[m_ParticlePool.m_iCurrentBuffer] == iMaxColorInterval) 
		m_iColorUpdateInterval[m_ParticlePool.m_iCurrentBuffer] = 0;
}

void CVolumetricCloud::SetEvolvingSpeed(float Speed)
{
	m_fEvolvingSpeed = Speed;
}

void CVolumetricCloud::SetSunColor(float r, float g, float b, float i)
{
	m_cSunColor.r = r * i;
	m_cSunColor.g = g * i;
	m_cSunColor.b = b * i;
}

void CVolumetricCloud::SetWindVelocity(float v)
{
	m_vWindVelocity.z = v;
	m_ParticlePool.m_vWindVelocity.z = v;
}

void CVolumetricCloud::UpdateViewDistance()
{
	glm::vec3 relation_pos = m_vCloudPos+glm::vec3((float)(m_fLength/2.0),(float)(m_fHigh/2.0),(float)(m_fWidth/2.0)) - m_vViewpoint;
	m_fViewDistances = glm::length(relation_pos);
}
