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

#include "CloudParticle.h"
#include "../FSShaderProgramLoader.h"
#include "../Texture.h"
#include "../UniformWrapper.h"

class CVolumetricCloud;


struct PARTICLE_RENDERSTRUCT{
	glm::vec3 vecPos; // Coordinates of particle
	glm::vec2 vecOffset; // Vertex coordinate offsets
	float u, v; // Texture coordinates
	glm::vec4 Diffuse; // Diffuse color of particle
};

class CCloudShader{
private:
	CVolumetricCloud 		*m_pVolumetricCloud; 

	float					m_fParticleSize; // Size of particle
	FSShaderProgramLoader* loader;
	Texture* texture;

	UniformWrapper<glm::mat4> gWVP;
	UniformWrapper<glm::vec4> gRightNormal;
	UniformWrapper<glm::vec4> gUpNormal;
	UniformWrapper<GLuint>	  gSampler;

	GLuint VAO;
	GLuint VBO;
	GLuint IB;
	PARTICLE_RENDERSTRUCT* pVBData;
	int						m_iNumParticlesPerBuffer;

public:
	CCloudShader();
	~CCloudShader();
	bool Setup(const GLContext& context, CVolumetricCloud* VolumetricCloud, float fParticleSize, int iNumParticlesPerBuffer = 32);	
	void Cleanup();
	inline void SetParticleSize(float fSize){ m_fParticleSize = fSize;}	
	void SetParticleRS();
	void UnSetParticleRS();
	void Render();
	static inline DWORD FToW(float f){return *((DWORD*)&f);}
};


