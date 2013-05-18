#pragma once
#include "../gl.h"
#include "../ContextCallbacks.h"
#include "../Texture.h"
#include "../ControlledCamera.h"
#include "../FSShaderProgramLoader.h"
#include "../TransformMatrix.h"
#include "../UniformWrapper.h"
#include "utils.h"
#include "Primitives.h"

#include "GameScene.h"

#include <glm\ext.hpp>

ControlledCamera* gCamera;
PersProjInfo* gPersProjInfo;

class Clouds : public ContextCallbacks {
public:
	Clouds() {
	}

	~Clouds(){
	}

	bool hasIdleFunc() { return true; }

private:

	void initImpl(){
		int width  = glutGet(GLUT_WINDOW_WIDTH);
		int height = glutGet(GLUT_WINDOW_HEIGHT);
		GLfloat aspect = static_cast<GLfloat>(width) / height;
		gCamera = new ControlledCamera(width, height);
		gCamera->setPos(glm::vec3(0.0f, 0.0f, 0.0f));
		gCamera->setUp(glm::vec3(0.0f, -1.0f, 0.0f));
		gCamera->setHorizontalAngle(glm::pi<float>());
		gCamera->setSpeed(1);
		gCamera->Update();

		gPersProjInfo = new PersProjInfo();
		gPersProjInfo->FOV = 60.0f;
		gPersProjInfo->Height = (float)height;
		gPersProjInfo->Width = (float)width;
		gPersProjInfo->zNear = 0.1f;
		gPersProjInfo->zFar = 100.0f; 

		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

		fprintf(
			stdout,
			"INFO: OpenGL Version: %s\n",
			glGetString(GL_VERSION)
			);

		// setup gl
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glFrontFace(GL_CW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);

		m_gameScene = new CGameScene();
		m_gameScene->Setup(getContext());

		ExitOnGLError("Init failed");

	}

	void mouseImpl(int button, int state, int x, int y) {
		
	}

	void passiveMotionImpl(int x, int y) {		
		gCamera->mouseMotionFunc(x,y);
	}


	void displayImpl() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_gameScene->Render(0.0,0.0f);

	}

	void keyboardImpl(unsigned char key, int x, int y) {
		gCamera->keyboardFunc(key, x, y);
	}

	void idle() {
		gCamera->idleFunc();
	}


	void reshapeImpl(int width, int height) {
		glViewport(0, 0, width, height);
		gCamera->reshapeFunc(width, height);
		gPersProjInfo->Height = (float)height;
		gPersProjInfo->Width = (float)width;
		ExitOnGLError("Reshape failed");
	}


private:
	CGameScene* m_gameScene;
};