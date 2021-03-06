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


#include "..\gl.h"
#include "utils.h"

#define SUPPLY_INTERVAL 5	//the number of frames passed supply vapoar 
#define EXTINCT_FACTOR 0.1	
#define fRANDOM			(((float)rand())/RAND_MAX) //this is not thread safe and will not be as random as it could be
#define INDEX(i,j,k)	((i)*m_iHeight*m_iWidth+(j)*m_iWidth+(k))
#define SQUARE(r)		((r)*(r))


class CSimulationSpace
{
	friend class Geometry;
public:
	int 		m_iLength, m_iWidth, m_iHeight;
	UINT		m_uNumCellInVolume;
	UINT		m_uTotalNumCells;
protected:
	PBYTE		m_pShapeSpace;
	PFLOAT		m_pHumProbSpace;
	PFLOAT		m_pExtProbSpace;
	PFLOAT		m_pActProbSpace;

	UINT		m_uLastPhaseIndex; 
	UINT		m_uNextPhaseIndex; 
	PBYTE		m_apHumSpace[2];
	PBYTE		m_apCldSpace[2];
	PBYTE		m_apActSpace[2];
	PBYTE		m_pActFactorSpace;

	PFLOAT		m_apDensitySpace[2];
	PFLOAT		m_pMidDensitySpace;
	PFLOAT		m_pCurrentDensitySpace;
	int			m_iElapsedSteps;


public:
	CSimulationSpace();
	virtual ~CSimulationSpace();
	bool	Setup( UINT uLength, UINT uWidth, UINT uHigh );
	void	Cleanup();
	void	ShapeVolume();
	void	InitProbSpace();

	void	GrowCloud(UINT uPhaseIndex );
	void	ExtinctCloud( UINT uPhaseIndex);
	void	SupplyVapor(UINT uPhaseIndex);

	void	InterpolateDensitySpace( float fAlpha);
	float	GetCellDensity( int i, int j, int k );
	float	GetPointDensity( glm::vec3 *pvPoint );

	bool	IsCellInVolume( int i, int j, int k, int* pIndex = NULL );
	bool	IsCellInSpace( int i, int j, int k );
	void	InitCellInVolume( int i, int j, int k, float fProbSeed );
	bool	IsPointInSpace( glm::vec3 *pvPoint );
	UINT	GetNumCellInVolume(){ return m_uNumCellInVolume; }

protected:
	void	SetByteCell( PBYTE pByteSpace, int i, int j, int k, BYTE value);
	BYTE	GetByteCell( PBYTE pByteSpace, int i, int j, int k );
	bool	NewByteSpace( int size, PBYTE *ppBitSpace );
	bool	NewFloatSpace( int size, PFLOAT *ppFloatSpace );
	void	UpdateActFactorSpace(UINT uPhaseIndex);
	void	CelluarAutomate(UINT uPhaseIndex);
	void	CalculateDensity( UINT uPhaseIndex );
};
