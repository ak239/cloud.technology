#pragma once

#include "..\gl.h"
#include "..\UniformWrapper.h"
#include "CubemapTexture.h"
#include "..\Camera.h"
#include "..\ShaderProgram.h"
#include "..\FSShaderProgramLoader.h"
#include "..\PersProjInfo.h"
#include "..\TransformMatrix.h"
#include "Primitives.h"
#include "Mesh.h"

class SkyBox
{
public:
	SkyBox(const GLContext & context, Camera* pCamera, const PersProjInfo& p);

	~SkyBox();

	bool Init(const string& Directory,
		const string& PosXFilename,
		const string& NegXFilename,
		const string& PosYFilename,
		const string& NegYFilename,
		const string& PosZFilename,
		const string& NegZFilename);

	void Render();

private:    
	Camera* m_pCamera;
	CubemapTexture* m_pCubemapTex;
	Mesh* m_pMesh;
	PersProjInfo m_persProjInfo;
	const GLContext& m_Context;
	FSShaderProgramLoader* loader;

	UniformWrapper<glm::mat4> gWVP;
	UniformWrapper<GLuint>    gCubemapTexture;
};