#pragma once

#include "gl.h"
#include <math.h>
#include "ContextCallbacks.h"
#include "PointTutorial.h"

class  UniformTutorial : public ContextCallbacks
{
public:
	UniformTutorial(GLuint _gScaleLocation):gScaleLocation(_gScaleLocation){}
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

//		compileShader();
	}

	void displayImpl()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(0);

		glutSwapBuffers();
	}

	void idleImpl()
	{
		static float Scale = 0.0f;
		Scale += 0.001f;
		glUniform1f(gScaleLocation, sinf(Scale));
	}

	void addShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	void compileShader();

	GLuint VBO;
	GLuint gScaleLocation;
};

