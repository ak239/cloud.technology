#pragma once

#include "ContextCallbacks.h"
#include "PointTutorial.h"
#include "MoveTutorial.h"

class InterpolationTutorial : public ContextCallbacks
{
public:
	InterpolationTutorial(GLuint _gWorldLocation):gWorldLocation(_gWorldLocation), scale(0.0f){}
	bool hasIdleFunc(){ return true; }

private:
	void initImpl()
	{
		Vector3f Vertices[3];
		Vertices[0] = Vector3f(-1.0f, -1.0f, 0.0f);
		Vertices[1] = Vector3f(1.0f, -1.0f, 0.0f);
		Vertices[2] = Vector3f(0.0f, 1.0f, 0.0f);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	}

	void displayImpl()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		Matrix4f World;

		World.m[0][0]=sinf(scale); World.m[0][1]=0.0f;        World.m[0][2]=0.0f;        World.m[0][3]=0.0f;
		World.m[1][0]=0.0f;        World.m[1][1]=cosf(scale); World.m[1][2]=0.0f;        World.m[1][3]=0.0f;
		World.m[2][0]=0.0f;        World.m[2][1]=0.0f;        World.m[2][2]=sinf(scale); World.m[2][3]=0.0f;
		World.m[3][0]=0.0f;        World.m[3][1]=0.0f;        World.m[3][2]=0.0f;        World.m[3][3]=1.0f;

		glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &World.m[0][0]);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(0);
	}

	void idleImpl()
	{
		scale += 0.001f;
	}

	GLuint VBO;
	GLuint gWorldLocation;
	float  scale;
};

