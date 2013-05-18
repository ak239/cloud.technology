#pragma once

#include "Mesh.h"

static Mesh* createCube() {
	MeshEntry* entry = new MeshEntry();

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Vertex VERTICES[24] =
	{
		Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f) ),
		Vertex(glm::vec3(-1.0f, -1.0f, +1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f) ), 
		Vertex(glm::vec3(+1.0f, -1.0f, +1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f) ), 
		Vertex(glm::vec3(+1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f) ),
		
		Vertex(glm::vec3(-1.0f, +1.0f, -1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f ) ), 
		Vertex(glm::vec3(-1.0f, +1.0f, +1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f ) ), 
		Vertex(glm::vec3(+1.0f, +1.0f, +1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f ) ), 
		Vertex(glm::vec3(+1.0f, +1.0f, -1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f ) ),
		
		Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f) ), 
		Vertex(glm::vec3(-1.0f, +1.0f, -1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f) ), 
		Vertex(glm::vec3(+1.0f, +1.0f, -1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f) ), 
		Vertex(glm::vec3(+1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f) ),
		
		Vertex(glm::vec3(-1.0f, -1.0f, +1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f ) ), 
		Vertex(glm::vec3(-1.0f, +1.0f, +1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f ) ), 
		Vertex(glm::vec3(+1.0f, +1.0f, +1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f ) ), 
		Vertex(glm::vec3(+1.0f, -1.0f, +1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f ) ),
		
		Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f) ), 
		Vertex(glm::vec3(-1.0f, -1.0f, +1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f) ), 
		Vertex(glm::vec3(-1.0f, +1.0f, +1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f) ), 
		Vertex(glm::vec3(-1.0f, +1.0f, -1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f) ),

		Vertex(glm::vec3(+1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f ) ), 
		Vertex(glm::vec3(+1.0f, -1.0f, +1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f ) ), 
		Vertex(glm::vec3(+1.0f, +1.0f, +1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f ) ), 
		Vertex(glm::vec3(+1.0f, +1.0f, -1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f ) ) 

	};

	for (int i = 0; i < 24; i++) {
		vertices.push_back(VERTICES[i]);
	}

	const GLuint INDICES[36] =
	{
		0,2,1, 0,3,2, 
		4,5,6, 4,6,7, 
		8,9,10, 8,10,11, 
		12,15,14, 12,14,13, 
		16,17,18, 16,18,19, 
		20,23,22, 20,22,21
	};

	for (int i = 0; i < 36; i++)
	{
		indices.push_back(INDICES[i]);
	}

	entry->Init(vertices, indices);

	std::vector<MeshEntry*> entries;
	entries.push_back(entry);
	Mesh* cube = new Mesh(entries);
	return cube;
}