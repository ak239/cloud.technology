#include "Skybox.h"

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
	loader = new FSShaderProgramLoader("./sdk/clouds/background");
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

	glDepthFunc(GL_LEQUAL);

	GLContext context = GLContext::getCurrentContext();
	glm::mat4 mCamera = context.getCamera()->cameraMat();

	TransformMatrix tm;
	tm.setWorldPos(m_pCamera->getPos());
	tm.setRotate(glm::vec3(90.0f, 90.0f, 90.0f));
	tm.setCamera(mCamera);
	tm.setProjection(glm::perspective(m_persProjInfo.FOV, 
		m_persProjInfo.Width / m_persProjInfo.Height, 
		m_persProjInfo.zNear, 
		m_persProjInfo.zFar));

	glm::mat4 mTransform = tm.getTransform();
	gWVP.setValue(mTransform);

	m_pCubemapTex->Bind(GL_TEXTURE0);
	m_pMesh->Render();  

	glCullFace(OldCullFaceMode);        
	glDepthFunc(OldDepthFuncMode);
}