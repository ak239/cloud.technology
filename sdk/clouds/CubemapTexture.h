#pragma once

#include <string>
#include "..\gl.h"


using std::string;

class CubemapTexture
{
public:

	CubemapTexture(const string& Directory,
		const string& PosXFilename,
		const string& NegXFilename,
		const string& PosYFilename,
		const string& NegYFilename,
		const string& PosZFilename,
		const string& NegZFilename);

	~CubemapTexture();

	bool Load();

	void Bind(GLenum TextureUnit);

private:

	string m_fileNames[6];
	GLuint m_textureObj;
};