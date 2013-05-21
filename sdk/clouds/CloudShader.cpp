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
#include "..\GLContext.h"

using namespace std;

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
	gSampler.setValue(0);
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

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*m_iNumParticlesPerBuffer*sizeof(unsigned short), indices, GL_DYNAMIC_DRAW);
	
	delete indices;
	ExitOnGLError("CloudShader : Can't create VBO");

	glBindVertexArray(0);

	pVBData = new PARTICLE_RENDERSTRUCT[4*m_iNumParticlesPerBuffer];

	return true;

}


void CCloudShader::Cleanup(){

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IB);
	glDeleteVertexArrays(1, &VAO);

	delete pVBData;
	delete texture;
	delete loader;
}

void CCloudShader::Render()
{
	PersProjInfo* projInfo = GLContext::getCurrentContext().getPersProjInfo();

	glEnable( GL_BLEND );
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	loader->getProgram().use();
	texture->bind(GL_TEXTURE0);
	
	TransformMatrix tm;
	//glm::mat4 mCamera = glm::lookAt(gCamera->getPos(), glm::vec3(-642.0, 510.0, -788.0), gCamera->getUp());
	GLContext context = GLContext::getCurrentContext();
	glm::mat4 mCamera = context.getCamera()->cameraMat();
	tm.setCamera(mCamera);
	tm.setProjection(glm::perspective(projInfo->FOV, projInfo->Width/projInfo->Height, projInfo->zNear, projInfo->zFar));

	glm::mat4 mTransform = tm.getTransform();
	gWVP.setValue(mTransform);

	glm::vec4 vecRight = glm::vec4(mCamera[0][0], mCamera[1][0], mCamera[2][0], 0.0f);
	glm::vec4 vecUp = glm::vec4(mCamera[0][1], mCamera[1][1], mCamera[2][1], 0.0f);

	gRightNormal.setValue(glm::normalize(vecRight));
	gUpNormal.setValue(glm::normalize(vecUp));

	int uNumInBlock = 0;
	CParticleEnumerator Enumerator( & m_pVolumetricCloud->m_ParticlePool );
	CloudParticle *pCurParticle = Enumerator.NextParticleFromLastBuffer();
	while( pCurParticle )
	{
		if( pCurParticle->m_bVisible )
		{
			pVBData[4*uNumInBlock].vecPos = *( pCurParticle->GetPositionFromLastBuffer() );
			pVBData[4*uNumInBlock].vecOffset = glm::vec2(-m_fParticleSize,m_fParticleSize);
			pVBData[4*uNumInBlock].Diffuse = pCurParticle->m_cScatteringColor;
			pVBData[4*uNumInBlock].u = 0.0f; 
			pVBData[4*uNumInBlock].v = 0.0f;
					
			pVBData[4*uNumInBlock+1].vecPos =  *( pCurParticle->GetPositionFromLastBuffer() );
			pVBData[4*uNumInBlock+1].vecOffset = glm::vec2(m_fParticleSize, m_fParticleSize);
			pVBData[4*uNumInBlock+1].Diffuse = pCurParticle->m_cScatteringColor;
			pVBData[4*uNumInBlock+1].u = 1.0f; 
			pVBData[4*uNumInBlock+1].v = 0.0f;
					
			pVBData[4*uNumInBlock+2].vecPos =  *( pCurParticle->GetPositionFromLastBuffer() );
			pVBData[4*uNumInBlock+2].vecOffset = glm::vec2(-m_fParticleSize, -m_fParticleSize);
			pVBData[4*uNumInBlock+2].Diffuse = pCurParticle->m_cScatteringColor;
			pVBData[4*uNumInBlock+2].u = 0.0f; 
			pVBData[4*uNumInBlock+2].v = 1.0f;
					
			pVBData[4*uNumInBlock+3].vecPos =  *( pCurParticle->GetPositionFromLastBuffer() );
			pVBData[4*uNumInBlock+3].vecOffset = glm::vec2(m_fParticleSize, -m_fParticleSize);
			pVBData[4*uNumInBlock+3].Diffuse = pCurParticle->m_cScatteringColor;
			pVBData[4*uNumInBlock+3].u = 1.0f; 
			pVBData[4*uNumInBlock+3].v = 1.0f;

			uNumInBlock ++;
			if (uNumInBlock >= m_iNumParticlesPerBuffer)
			{
				glBindVertexArray(VAO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferSubData(GL_ARRAY_BUFFER, 0, 4*m_iNumParticlesPerBuffer*sizeof(PARTICLE_RENDERSTRUCT), pVBData);
				glEnableVertexAttribArray(0);
				glEnableVertexAttribArray(1);
				glEnableVertexAttribArray(2);
				glEnableVertexAttribArray(3);

				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PARTICLE_RENDERSTRUCT), (const GLvoid*)0);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(PARTICLE_RENDERSTRUCT), (const GLvoid*)12);
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(PARTICLE_RENDERSTRUCT), (const GLvoid*)20);
				glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(PARTICLE_RENDERSTRUCT), (const GLvoid*)28);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

				glDrawElements(GL_TRIANGLES, 6*m_iNumParticlesPerBuffer, GL_UNSIGNED_SHORT, (GLvoid*)0);

				uNumInBlock = 0;
				ExitOnGLError("CloudShader rendering failed");
			}
		}
		// Go to next particle
		pCurParticle = Enumerator.NextParticleFromLastBuffer();
	}

	if (uNumInBlock != 0)
	{
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 4*uNumInBlock*sizeof(PARTICLE_RENDERSTRUCT), pVBData);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PARTICLE_RENDERSTRUCT), (const GLvoid*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(PARTICLE_RENDERSTRUCT), (const GLvoid*)12);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(PARTICLE_RENDERSTRUCT), (const GLvoid*)20);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(PARTICLE_RENDERSTRUCT), (const GLvoid*)28);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

		glDrawElements(GL_TRIANGLES, 6*uNumInBlock, GL_UNSIGNED_SHORT, (GLvoid*)0);
	}

	
	
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}
