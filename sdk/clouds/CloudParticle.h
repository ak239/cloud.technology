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

#pragma once

#include <vector>
#include <algorithm>
#include "../gl.h"

//-----------------------------------------------------------------------------
// Forward declarations
class CVolumetricCloud;
class CParticlePool;
class CParticleEnumerator;


class CloudParticle
{
public:
	CParticlePool *m_pParticlePool;
	UINT		m_i,m_j,m_k;
	UINT		m_uIndex;
	glm::vec4	m_cScatteringColor;
	bool		m_bVisible;
public:
	CloudParticle();
	CloudParticle(UINT i, UINT j, UINT k,CParticlePool *pParticlePool);
	~CloudParticle();
	glm::vec3* GetPosition();
	glm::vec3* GetPositionFromLastBuffer();
	double		 GetViewDistance();
};


class CParticlePool
{
	friend class CParticleEnumerator;
	friend class CloudParticle;
public:
	CParticlePool();
	~CParticlePool();
	void SortbyViewDistances(const glm::vec3 &vLookDir);
	void UpdateParticlePositions( double fTime );
	bool AddParticle(UINT i, UINT j, UINT k);
	bool Setup( CVolumetricCloud *pVolumetricCloud, UINT uNumParticles );
	void Cleanup();

protected:
	CVolumetricCloud *m_pVolumetricCloud;
	std::vector< CloudParticle* >	m_v_pCloudParticles[2];
	UINT			m_uNumParticles;	
	glm::vec3		*m_pvPositions[2];
	double			*m_pfViewDistances;
	double          m_PreTime[2];

public:
	glm::vec3     m_vWindVelocity;
	UINT            m_iCurrentBuffer;

};


class CParticleEnumerator
{
public:
	CParticleEnumerator( CParticlePool *pParticlePool );
	~CParticleEnumerator();
	CloudParticle* NextParticle();
	CloudParticle* NextParticleFromLastBuffer();
	void Reset();
private:
	CParticlePool *m_pParticlePool;
	UINT m_uIndex;
};
