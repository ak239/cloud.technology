#pragma once

#include "gl.h"
#include "ContextCallbacks.h"
#include "ControlledCamera.h"
#include <glm\gtc\type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "RotateTutorial.h"

class TransformTutorial : public ContextCallbacks
{
public:
	TransformTutorial(GLuint _gWorldLocation, GLint width, GLint height) : step(0.0), gWorldLocation(_gWorldLocation), c(width, height){}
	bool hasIdleFunc() { return true; }

private:
	void initImpl()
	{
		glm::mat4x3 vertices;
		vertices[0] = glm::vec3(-1.0, -1.0, 0.0);
		vertices[1] = glm::vec3(0.0,  -1.0, 1.0);
		vertices[2] = glm::vec3(1.0,  -1.0, 0.0);
		vertices[3] = glm::vec3(0.0,   1.0, 0.0);

		glGenBuffers(1, &vertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.col_size() * vertices.row_size(), glm::value_ptr(vertices), GL_STATIC_DRAW);

		unsigned int indices[] = { 0, 3, 1,
									1, 3, 2,
									2, 3, 0,
									0, 2, 1 };

		glGenBuffers(1, &indexesBufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexesBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glm::vec3 CameraPos(0.0f, 0.0f, -3.0f);
		glm::vec3 CameraTarget(0.0f, 0.0f, 2.0f);
		glm::vec3 CameraUp(0.0f, 1.0f, 0.0f);
		c.setPos(CameraPos);
		c.setVerticalAngle(0.0f);
		c.setHorizontalAngle(0.0f);
		c.setUp(CameraUp);
	}

	void displayImpl()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 m = glm::perspective(30.0f, 1.0f, 1.0f, 1000.0f);
		m *= c.cameraMat();
		m = glm::translate(m, glm::vec3(0.0f, 0.0f, -5.0f));
		m = glm::rotate(m, step, glm::vec3(0.0f, 1.0f, 0.0f));
		m = glm::scale(m, glm::vec3(0.5f));

		//30.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 1000.0f)

		glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &m[0][0]);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexesBufferObject);

		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(0);
	}

	void idleImpl()
	{
		step += 0.003f;
//		c.idleFunc();
	}

	void keyboardImpl(unsigned char key, int x, int y)
	{
		c.keyboardFunc(key, x, y);
	}

	void motionImpl(int x, int y)
	{
		c.mouseMotionFunc(x, y);			
	}

	void passiveMotionImpl(int x, int y)
	{
		c.mouseMotionFunc(x, y);			
	}
	
	float step;
	GLuint gWorldLocation;
	GLuint vertexBufferObject;
	GLuint indexesBufferObject;
	ControlledCamera c;
};

