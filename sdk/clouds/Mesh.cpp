#include "Mesh.h"

#define INVALID_OGL_VALUE 0xFFFFFFFF

MeshEntry::MeshEntry()
{
	VB = INVALID_OGL_VALUE;
	IB = INVALID_OGL_VALUE;
	NumIndices  = 0;
};

MeshEntry::~MeshEntry()
{
	printf("Deleted Mesh Entry");
	if (VB != INVALID_OGL_VALUE)
	{
		glDeleteBuffers(1, &VB);
	}

	if (IB != INVALID_OGL_VALUE)
	{
		glDeleteBuffers(1, &IB);
	}
}

bool MeshEntry::Init(const std::vector<Vertex>& Vertices,
						   const std::vector<unsigned int>& Indices)
{
	NumIndices = Indices.size();

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * NumIndices, &Indices[0], GL_STATIC_DRAW);
	return false;
}

void Mesh::Clear()
{
}

void Mesh::Render()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	ExitOnGLError("Mesh.cpp : Can't enable vertex attribute");

	for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
		glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i]->VB);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) 12);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) 20);
		ExitOnGLError("Mesh.cpp : Can't update vertex attribute");

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i]->IB);

		/*const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

		if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex]) {
		m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
		}*/

		glDrawElements(GL_TRIANGLES, m_Entries[i]->NumIndices, GL_UNSIGNED_INT, 0);
		ExitOnGLError("Mesh.cpp : Can't draw mesh");
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}