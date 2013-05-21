#pragma once

#include "gl.h"
#include "ContextCallbacks.h"
#include "FSShaderProgramLoader.h"
#include "ControlledCamera.h"
#include "FSShaderProgramLoader.h"
#include "Texture.h"
#include "TransformMatrix.h"

class TextureTutorial : public ContextCallbacks
{
public:
	TextureTutorial():camera(nullptr), texture(nullptr), var(0.0f){}
	~TextureTutorial(){
		delete camera;
		delete texture;
	}

	bool hasIdleFunc() { return true; }

private:

	void initImpl(){
		int width  = glutGet(GLUT_WINDOW_WIDTH);
		int height = glutGet(GLUT_WINDOW_HEIGHT);
		aspect = static_cast<GLfloat>(width) / height;
		// create camera
		camera = new ControlledCamera(width, height);
		camera->setPos(glm::vec3(0.0f, 0.0f, 0.0f));
		camera->setUp(glm::vec3(0.0f, 1.0f, 0.0f));
		
		// setup gl
		glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
		glFrontFace(GL_CW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		
		// create buffers
		createVertexBuffer();
		createIndexBuffer();
		
		// load shaders
		FSShaderProgramLoader loader("./texture_tutorial");
		if (loader.loadProgram(getContext())){
			const ShaderProgram& program = loader.getProgram();
			program.use();
			gWVP     = program.getUniformLoc("gWVP");
			gSampler = program.getUniformLoc("gSampler");
		}else
			std::cerr << loader.getLastError() << std::endl;

		// load texture
		texture = new Texture(getContext(), GL_TEXTURE_2D, "test.png");
		if (!texture->load())
			std::cerr << "Can't load texture: " << texture->getLastError();
	}

	void idleImpl()
	{
		var += 0.1f;
//		camera->idleFunc();
	}

	void keyboardImpl(unsigned char key, int x, int y)
	{
		if (key == 'q')
			glutLeaveMainLoop();

		camera->keyboardFunc(key, x, y);
	}

	void passiveMotionImpl(int x, int y)
	{
		camera->mouseMotionFunc(x, y);			
	}

	void displayImpl(){
		glClear(GL_COLOR_BUFFER_BIT);

		TransformMatrix tm;
		tm.setRotate(glm::vec3(0.0f, 0.0f, 0.0f));
		tm.setWorldPos(glm::vec3(0.0f, 0.0f, 0.0f));
		tm.setCamera(camera->cameraMat());
		tm.setProjection(glm::perspective(60.0f, aspect, 1.0f, 100.0f));
		
		glUniformMatrix4fv(gWVP, 1, GL_TRUE, &tm.getTransform()[0][0]);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat[5]), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat[5]), (const GLvoid*)12);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
		texture->bind(GL_TEXTURE0);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		GLenum error = glGetError();
		if (error != 0)
			std::cerr << error << std::endl;;
	}

	void createVertexBuffer(){
		GLfloat vertices[] = { 
			-1.0f, -1.0f, 0.5773f,   0.0f, 0.0f,
			0.0f,  -1.0f, -1.15475f, 0.5f, 0.0f,
			1.0f,  -1.0f, 0.5773f,   1.0f, 0.0f,
			0.0f,  1.0f,  0.0f,      0.5f, 1.0f 
		};
		glGenBuffers(1, &vertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	}

	void createIndexBuffer(){
		GLuint indices[] = { 0, 3, 1, 1, 3, 2, 2, 3 , 0, 1, 2 ,0 };
		glGenBuffers(1, &indexBufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}

private:
	ControlledCamera* camera;
	
	GLuint vertexBufferObject;
	GLuint indexBufferObject;

	GLuint gWVP;
	GLuint gSampler;

	Texture* texture;

	GLfloat var;
	GLfloat aspect;
};
