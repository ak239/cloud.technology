#include "Skybox.h"

#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }

SkyBox::SkyBox(const GLContext & context, Camera* pCamera,
			   const PersProjInfo& p) : m_Context(context)
{
	m_pCamera = pCamera;
	m_persProjInfo = p;            

	m_pCubemapTex = NULL;
	m_pMesh = NULL;
}


SkyBox::~SkyBox()
{
	SAFE_DELETE(m_pCubemapTex);
	SAFE_DELETE(m_pMesh);
}

bool SkyBox::Init(const string& Directory,
				  const string& PosXFilename,
				  const string& NegXFilename,
				  const string& PosYFilename,
				  const string& NegYFilename,
				  const string& PosZFilename,
				  const string& NegZFilename)
{
	loader = new FSShaderProgramLoader("./clouds/background");
	if (loader->loadProgram(m_Context)){
		const ShaderProgram& program = loader->getProgram();
		program.use();
		gWVP               = UniformWrapper<glm::mat4>(program.getUniformLoc("gWVP"));
		gCubemapTexture    = UniformWrapper<GLuint>(program.getUniformLoc("gCubemapTexture"));
	}else {
		printf("Error initializing skybox");
		std::cerr << loader->getLastError() << std::endl;
		return false;
	}

	ShaderProgram const & program = loader->getProgram();
	
	program.use();

	gCubemapTexture.setValue(0);

	m_pCubemapTex = new CubemapTexture(Directory,
		PosXFilename,
		NegXFilename,
		PosYFilename,
		NegYFilename,
		PosZFilename,
		NegZFilename);

	if (!m_pCubemapTex->Load()) {
		return false;
	}

	m_pMesh = createCube();

	return true;
}


void SkyBox::Render()
{
	ShaderProgram const & program  = loader->getProgram();
	program.use();

	GLint OldCullFaceMode;
	glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
	GLint OldDepthFuncMode;
	glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

	//glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);

	TransformMatrix tm;
	float scale = 20.f;
	tm.setScale( glm::vec3(scale));
	tm.setRotate(glm::vec3(0.0f, 0.0f, 0.0f));
	tm.setWorldPos(m_pCamera->getPos());
	tm.setCamera(m_pCamera->cameraMat());
	tm.setCameraPosition(m_pCamera->getPos());
	tm.setProjection(glm::perspective(m_persProjInfo.FOV, 
		m_persProjInfo.Width / m_persProjInfo.Height, 
		m_persProjInfo.zNear, 
		m_persProjInfo.zFar));

	glm::mat4 transformMatrix = tm.getTransform();
	glm::vec4 v = transformMatrix * glm::vec4(0.0, 0.0, 0.0, 1.0);
	//printf("(%f, %f, %f, %f)\n", v.x, v.y, v.z, v.w);
	gWVP.setValue(transformMatrix);

	m_pCubemapTex->Bind(GL_TEXTURE0);
	m_pMesh->Render();  

	glCullFace(OldCullFaceMode);        
	glDepthFunc(OldDepthFuncMode);
}