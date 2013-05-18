#pragma once

#include <vector>
#include "..\gl.h"
#include "utils.h"

struct Vertex
{
	glm::vec3 m_pos;
	glm::vec2 m_tex;
	glm::vec3 m_normal;

	Vertex() {}

	Vertex(const glm::vec3& pos, const glm::vec2& tex, const glm::vec3& normal)
	{
		m_pos    = pos;
		m_tex    = tex;
		m_normal = normal;
	}
};

struct MeshEntry{
	MeshEntry();
	~MeshEntry();

	bool Init(const std::vector<Vertex>& Vertices,
		const std::vector<unsigned int>& Indices);

	GLuint VB;
	GLuint IB;

	unsigned int NumIndices;
};

class Mesh
{
public:
	Mesh(std::vector<MeshEntry*> const & entries) : m_Entries(entries) {};
	~Mesh(){
		for (size_t i = 0; i < m_Entries.size(); i++) {
			delete m_Entries[i];
		}

		Clear();
	};
	void Render();

private:
	void Clear();

	std::vector<MeshEntry*> m_Entries;
};