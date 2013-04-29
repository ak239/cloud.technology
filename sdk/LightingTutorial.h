#pragma once

#include "gl.h"
#include "ContextCallbacks.h"
#include "Texture.h"
#include "ControlledCamera.h"
#include "FSShaderProgramLoader.h"
#include "TransformMatrix.h"
#include "UniformWrapper.h"

class LightingTutorial : public ContextCallbacks
{
public:
	LightingTutorial():camera(nullptr), texture(nullptr), var(0.0f){}
	~LightingTutorial(){
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
		createBuffers();
		
		// load shaders
		FSShaderProgramLoader loader("./lighting_tutorial");
		if (loader.loadProgram(getContext())){
			const ShaderProgram& program = loader.getProgram();
			program.use();
			gWVP               = UniformWrapper<glm::mat4>(program.getUniformLoc("gWVP"));
			gWorld             = UniformWrapper<glm::mat4>(program.getUniformLoc("gWorld"));
			gSampler           = UniformWrapper<GLuint>(program.getUniformLoc("gSampler"));
			gLightColor        = UniformWrapper<glm::vec3>(program.getUniformLoc("gDirectionalLight.Color"));
			gLightIntensity    = UniformWrapper<GLfloat>(program.getUniformLoc("gDirectionalLight.AmbientIntensity"));
			gLightDirection    = UniformWrapper<glm::vec3>(program.getUniformLoc("gDirectionalLight.Direction"));
			gDirLightIntensity = UniformWrapper<GLfloat>(program.getUniformLoc("gDirectionalLight.DiffuseIntensity"));
		}else
			std::cerr << loader.getLastError() << std::endl;

		gLightColor.setValue(glm::vec3(1.0f, 1.0f, 1.0f));
		gLightIntensity.setValue(0.0f);
		gLightDirection.setValue(glm::vec3(1.0f, 0.0f, 0.0f));
		gDirLightIntensity.setValue(0.75f);

		// load texture
		texture = new Texture(getContext(), GL_TEXTURE_2D, "test.png");
		if (!texture->load())
			std::cerr << "Can't load texture: " << texture->getLastError();
	}

	void idleImpl()
	{
		var += 0.1f;
		camera->idleFunc();
	}

	void keyboardImpl(unsigned char key, int x, int y)
	{
		if (key == 'q')
			glutLeaveMainLoop();
		switch(key)
		{
			case 'a':
				gLightIntensity.setValue(gLightIntensity.getValue() - 0.05f);
			break;

			case 's':
				gLightIntensity.setValue(gLightIntensity.getValue() + 0.05f);
			break;

			case 'z':
				gDirLightIntensity.setValue(gDirLightIntensity.getValue() + 0.05f);
				break;

			case 'x':
				gDirLightIntensity.setValue(gDirLightIntensity.getValue() - 0.05f);
				break;
		}
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
		
		gWVP.setValue(tm.getWVP());
		gWorld.setValue(tm.getTransform());

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat[8]), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat[8]), (const GLvoid*)12);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat[8]), (const GLvoid*)20);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
		texture->bind(GL_TEXTURE0);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		GLenum error = glGetError();
		if (error != 0)
			std::cerr << error << std::endl;;
	}

	glm::vec3 vec3ByFloatArr(GLfloat* arr){
		return glm::vec3(arr[0], arr[1], arr[2]);
	}

	void createBuffers(){
		GLfloat vertices[] = { 
			-1.0f, -1.0f, 0.5773f,   0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			0.0f,  -1.0f, -1.15475f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
			1.0f,  -1.0f, 0.5773f,   1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			0.0f,  1.0f,  0.0f,      0.5f, 1.0f, 0.0f, 0.0f, 0.0f 
		};
		GLuint indices[] = { 0, 3, 1, 1, 3, 2, 2, 3 , 0, 1, 2 ,0 };

		for (int i = 0; i < sizeof(vertices) / sizeof(GLfloat[8]); i += 3){
			GLuint idx1 = indices[i];
			GLuint idx2 = indices[i + 1];
			GLuint idx3 = indices[i + 2];

			glm::vec3 v1 = vec3ByFloatArr(vertices + idx2 * 8) - vec3ByFloatArr(vertices + idx1 * 8);
			glm::vec3 v2 = vec3ByFloatArr(vertices + idx3 * 8) - vec3ByFloatArr(vertices + idx1 * 8);
			glm::vec3 n = glm::normalize(glm::cross(v1, v2));

			vertices[idx1 * 8]     += n.x;
			vertices[idx1 * 8 + 1] += n.y;
			vertices[idx1 * 8 + 2] += n.z;

			vertices[idx2 * 8]     += n.x;
			vertices[idx2 * 8 + 1] += n.y;
			vertices[idx2 * 8 + 2] += n.z;
			
			vertices[idx3 * 8]     += n.x;
			vertices[idx3 * 8 + 1] += n.y;
			vertices[idx3 * 8 + 2] += n.z;
		}
		for (int i = 0; i < sizeof(vertices) / sizeof(GLfloat[8]); ++i){
			glm::vec3 n = glm::normalize(vec3ByFloatArr(vertices + i * 8));
			vertices[i * 8]     += n.x;
			vertices[i * 8 + 1] += n.y;
			vertices[i * 8 + 2] += n.z;
		}
		
		glGenBuffers(1, &vertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &indexBufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	}

private:
	ControlledCamera* camera;
	
	GLuint vertexBufferObject;
	GLuint indexBufferObject;

	UniformWrapper<glm::mat4> gWVP;
	UniformWrapper<glm::mat4> gWorld;
	UniformWrapper<GLuint>    gSampler;
	UniformWrapper<glm::vec3> gLightColor;
	UniformWrapper<GLfloat>   gLightIntensity;
	UniformWrapper<glm::vec3> gLightDirection;
	UniformWrapper<GLfloat>   gDirLightIntensity;

	Texture* texture;

	GLfloat var;
	GLfloat aspect;
};
