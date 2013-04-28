#pragma once

#include "ContextCallbacks.h"

#include "gl.h"

struct Vector3f
{
	float x;
	float y;
	float z;
	Vector3f():x(0.0f), y(0.0f), z(0.0f){}
	Vector3f(float _x, float _y, float _z) : x(_x), y(_y), z(_z){}
};

class PointTutorial : public ContextCallbacks
{
public:
	PointTutorial() : VBO(0) {}

private:
	void initImpl()
	{
		Vector3f Vertices[1];
    
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	}

	void displayImpl()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glDrawArrays(GL_POINTS, 0, 1);

		glDisableVertexAttribArray(0);

		glutSwapBuffers();
	}

	GLuint VBO;
};

