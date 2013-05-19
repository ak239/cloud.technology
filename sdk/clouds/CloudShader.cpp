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

using namespace std;

extern ControlledCamera*  gCamera;

CCloudShader::CCloudShader()
{
}

CCloudShader::~CCloudShader()
{
	delete texture;
	delete loader;
}

bool CCloudShader::Setup( const GLContext& context, CVolumetricCloud* pVolumetricCloud, float fParticleSize, int iNumParticlesPerBuffer )
{
	m_pVolumetricCloud = pVolumetricCloud;
	// load shaders
	loader = new FSShaderProgramLoader("./clouds/particle");
	if (loader->loadProgram(context)){
		const ShaderProgram& program = loader->getProgram();
		program.use();
		gWVP               = UniformWrapper<glm::mat4>(program.getUniformLoc("gWVP"));
		gRightNormal       = UniformWrapper<glm::vec4>(program.getUniformLoc("rightNormal"));
		gUpNormal        = UniformWrapper<glm::vec4>(program.getUniformLoc("upNormal"));
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

	return true;

}


void CCloudShader::Cleanup(){
	delete texture;
	delete loader;
}

void CCloudShader::Render()
{
}
