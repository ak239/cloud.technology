#pragma once

#include "ContextCallbacks.h"
#include "PointTutorial.h"
#include "MoveTutorial.h"
#include <glm\gtc\type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class IndexesTutorial : public ContextCallbacks
{
public:
	IndexesTutorial(GLuint _gWorldLocation):gWorldLocation(_gWorldLocation), scale(0.0f){}
	bool hasIdleFunc(){ return true; }

private:
	void initImpl()
	{
		glm::mat4x3 vertices;
		vertices[0] = glm::vec3(-1.0, -1.0, 0.0);
		vertices[1] = glm::vec3(0.0,  -1.0, 1.0);
		vertices[2] = glm::vec3(1.0,  -1.0, 0.0);
		vertices[3] = glm::vec3(0.0,   1.0, 0.0);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.col_size() * vertices.row_size(), glm::value_ptr(vertices), GL_STATIC_DRAW);

		unsigned int Indices[] = { 0, 3, 1,
							1, 3, 2,
							2, 3, 0,
							0, 2, 1 };

		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
	}

	void displayImpl()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		Matrix4f World;

		World.m[0][0] = cosf(scale); World.m[0][1] = 0.0f; World.m[0][2] = -sinf(scale); World.m[0][3] = 0.0f;
		World.m[1][0] = 0.0;         World.m[1][1] = 1.0f; World.m[1][2] = 0.0f        ; World.m[1][3] = 0.0f;
		World.m[2][0] = sinf(scale); World.m[2][1] = 0.0f; World.m[2][2] = cosf(scale) ; World.m[2][3] = 0.0f;
		World.m[3][0] = 0.0f;        World.m[3][1] = 0.0f; World.m[3][2] = 0.0f        ; World.m[3][3] = 1.0f;

		glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &World.m[0][0]);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(0);
	}

	void idleImpl()
	{
		scale += 0.001f;
	}

	GLuint VBO;
	GLuint IBO;
	GLuint gWorldLocation;
	float  scale;
};
