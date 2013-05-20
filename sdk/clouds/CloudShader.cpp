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



#include "CloudShader.h"
#include "VolumetricCloud.h"
#include "..\ControlledCamera.h"
#include "..\PersProjInfo.h"
#include "..\TransformMatrix.h"

using namespace std;

extern ControlledCamera*  gCamera;
extern PersProjInfo* gPersProjInfo;

CCloudShader::CCloudShader()
{
}

CCloudShader::~CCloudShader()
{
}

bool CCloudShader::Setup( const GLContext& context, CVolumetricCloud* pVolumetricCloud, float fParticleSize, int iNumParticlesPerBuffer )
{
	m_pVolumetricCloud = pVolumetricCloud;
	m_fParticleSize = fParticleSize;
	m_iNumParticlesPerBuffer = iNumParticlesPerBuffer;

	// load shaders
	loader = new FSShaderProgramLoader("./clouds/particle");
	if (loader->loadProgram(context)){
		const ShaderProgram& program = loader->getProgram();
		program.use();
		gWVP               = UniformWrapper<glm::mat4>(program.getUniformLoc("gWVP"));
		gRightNormal       = UniformWrapper<glm::vec4>(program.getUniformLoc("rightNormal"));
		gUpNormal          = UniformWrapper<glm::vec4>(program.getUniformLoc("upNormal"));
		gSampler           = UniformWrapper<GLuint>(program.getUniformLoc("gSampler"));
	}else {
		std::cerr << loader->getLastError() << std::endl;
		return false;
	}

	// load texture
	texture = new Texture(context, GL_TEXTURE_2D, m_pVolumetricCloud->m_szTextureFile);
	if (!texture->load()) {
		std::cerr << "Can't load texture: " << texture->getLastError();
		return false;
	}

	ExitOnGLError("CloudShader initialization failed");

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	ExitOnGLError("CloudShader : Can't create VAO");
	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 4*m_iNumParticlesPerBuffer*sizeof(PARTICLE_RENDERSTRUCT), NULL, GL_DYNAMIC_DRAW);
	ExitOnGLError("CloudShader : Can't create VBO");

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	unsigned short* indices = new unsigned short[6*m_iNumParticlesPerBuffer];

	for (unsigned short i = 0; i < m_iNumParticlesPerBuffer; i++) 
	{
		indices[i*6+0] = (unsigned short)i * 4 + 0;
		indices[i*6+1] = (unsigned short)i * 4 + 1;
		indices[i*6+2] = (unsigned short)i * 4 + 2;
		indices[i*6+3] = (unsigned short)i * 4 + 3;
		indices[i*6+4] = (unsigned short)i * 4 + 2;
		indices[i*6+5] = (unsigned short)i * 4 + 1;
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*m_iNumParticlesPerBuffer*sizeof(unsigned short), indices, GL_STATIC_DRAW);
	
	delete indices;
	ExitOnGLError("CloudShader : Can't create VBO");

	return true;

}


void CCloudShader::Cleanup(){

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IB);
	glDeleteVertexArrays(1, &VAO);

	delete texture;
	delete loader;
}

void CCloudShader::Render()
{
	loader->getProgram().use();
	texture->bind(GL_TEXTURE0);

	TransformMatrix tm;
	tm.setCamera(gCamera->cameraMat());
	tm.setProjection(glm::perspective(gPersProjInfo->FOV, gPersProjInfo->Width/gPersProjInfo->Height, gPersProjInfo->zNear, gPersProjInfo->zFar));
	
	glm::mat4 mTransform = tm.getTransform();
	gWVP.setValue(mTransform);
	
	glm::vec4 vecRight = glm::vec4(mTransform[0][0], mTransform[1][0], mTransform[2][0], 0.0f);
	glm::vec4 vecUp = glm::vec4(mTransform[0][1], mTransform[1][1], mTransform[2][1], 0.0f);

	gRightNormal.setValue(glm::normalize(vecRight));
	gUpNormal.setValue(glm::normalize(vecUp));

	PARTICLE_RENDERSTRUCT* pVBData = new PARTICLE_RENDERSTRUCT[4*m_iNumParticlesPerBuffer];

	int uNumInBlock = 0;
	CParticleEnumerator Enumerator( & m_pVolumetricCloud->m_ParticlePool );
	CloudParticle *pCurParticle = Enumerator.NextParticleFromLastBuffer();
	while( pCurParticle )
	{
		if( pCurParticle->m_bVisible )
		{
			pVBData[uNumInBlock].vecPos = *( pCurParticle->GetPositionFromLastBuffer() );
			pVBData[uNumInBlock].vecOffset = glm::vec2(-m_fParticleSize,m_fParticleSize);
			pVBData[uNumInBlock].Diffuse = pCurParticle->m_cScatteringColor;
			pVBData[uNumInBlock].u = 0.0f; pVBData[0].v = 0.0f;

			pVBData[uNumInBlock+1].vecPos =  *( pCurParticle->GetPositionFromLastBuffer() );
			pVBData[uNumInBlock+1].vecOffset = glm::vec2(m_fParticleSize, m_fParticleSize);
			pVBData[uNumInBlock+1].Diffuse = pCurParticle->m_cScatteringColor;
			pVBData[uNumInBlock+1].u = 1.0f; pVBData[1].v = 0.0f;

			pVBData[uNumInBlock+2].vecPos =  *( pCurParticle->GetPositionFromLastBuffer() );
			pVBData[uNumInBlock+2].vecOffset = glm::vec2(-m_fParticleSize, -m_fParticleSize);
			pVBData[uNumInBlock+2].Diffuse = pCurParticle->m_cScatteringColor;
			pVBData[uNumInBlock+2].u = 0.0f; pVBData[2].v = 1.0f;

			pVBData[uNumInBlock+3].vecPos =  *( pCurParticle->GetPositionFromLastBuffer() );
			pVBData[uNumInBlock+3].vecOffset = glm::vec2(m_fParticleSize, -m_fParticleSize);
			pVBData[uNumInBlock+3].Diffuse = pCurParticle->m_cScatteringColor;
			pVBData[uNumInBlock+3].u = 1.0f; pVBData[3].v = 1.0f;

			uNumInBlock ++;
			if (uNumInBlock >= m_iNumParticlesPerBuffer)
			{
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferSubData(GL_ARRAY_BUFFER, 0, 4*m_iNumParticlesPerBuffer*sizeof(PARTICLE_RENDERSTRUCT), pVBData);
				glEnableVertexAttribArray(0);
				glEnableVertexAttribArray(1);
				glEnableVertexAttribArray(2);
				glEnableVertexAttribArray(3);
				
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PARTICLE_RENDERSTRUCT), (const GLvoid*)0);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(PARTICLE_RENDERSTRUCT), (const GLvoid*)12);
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(PARTICLE_RENDERSTRUCT), (const GLvoid*)20);
				glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(PARTICLE_RENDERSTRUCT), (const GLvoid*)28);

				glBindVertexArray(VAO);
				glDrawElements(GL_TRIANGLES, 2*m_iNumParticlesPerBuffer, GL_UNSIGNED_SHORT, (GLvoid*)0);

				uNumInBlock = 0;
				ExitOnGLError("CloudShader rendering failed");
			}
		}
		// Go to next particle
		pCurParticle = Enumerator.NextParticleFromLastBuffer();
	}

	if (uNumInBlock != 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 4*uNumInBlock*sizeof(PARTICLE_RENDERSTRUCT), pVBData);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 2*uNumInBlock, GL_UNSIGNED_SHORT, (GLvoid*)0);
	}
	
	
}
