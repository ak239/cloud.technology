#pragma once

#include <string>
#include "gl.h"
#include "GLContext.h"
#include <SOIL.h>

class Texture
{
public:
	Texture(const GLContext& _context, GLenum _target, const std::string& _fileName):
		context(_context), target(_target), fileName(_fileName), texId(0){}

	bool load(){
		GLContextGetter get(context);
		
		GLuint tmpTexId = SOIL_load_OGL_texture(fileName.c_str(), 
			SOIL_LOAD_AUTO, 
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
			);
		if (tmpTexId == 0){
			lastError = std::string("SOIL Error: ") + SOIL_last_result();
			return false;
		}

		glBindTexture(target, tmpTexId);
		glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		texId = tmpTexId;
		return true;
	}

	void bind(GLenum textureUnit){
		GLContextGetter get(context);
		glActiveTexture(textureUnit);
		glBindTexture(target, texId); 
	}

	~Texture(void){}

	const std::string& getLastError() const{ return lastError; }

private:
	GLContext   context;
	GLenum      target;
	std::string fileName;

	GLuint texId;

	std::string lastError;
};

