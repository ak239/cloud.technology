#pragma once

#include <string>
#include "gl.h"
#include "GLContext.h"
#include "nvImage.h"

class Texture
{
public:
	Texture(const GLContext& _context, GLenum _target, const std::string& _fileName):
		context(_context), target(_target), fileName(_fileName), texId(0){}

	bool load(){
		GLContextGetter get(context);
		
		//GLuint tmpTexId = SOIL_load_OGL_texture(fileName.c_str(), 
		//	SOIL_LOAD_AUTO, 
		//	SOIL_CREATE_NEW_ID,
		//	SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		//	//SOIL_FLAG_DDS_LOAD_DIRECT
		//	);
		//if (tmpTexId == 0){
		//	lastError = std::string("SOIL Error: ") + SOIL_last_result();
		//	return false;
		nv::Image fImage;
		fImage.loadImageFromFile(fileName.c_str());

		GLuint tmpTexId = 0;
		glGenTextures( 1, &tmpTexId);
		glBindTexture( GL_TEXTURE_2D, tmpTexId);

		glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		int w = fImage.getWidth();
		int h = fImage.getHeight();

		for (int i = 0; i < fImage.getMipLevels(); ++i){
			glTexImage2D( GL_TEXTURE_2D, i, fImage.getInternalFormat(), w, h, 0, fImage.getFormat(), fImage.getType(), fImage.getLevel(i));

            w = ( w > 1) ? w >> 1 : 1;
            h = ( h > 1) ? h >> 1 : 1;
		}
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

		texId = tmpTexId;
		return true;
	}

	void bind(GLenum textureUnit){
		GLContextGetter get(context);
		glActiveTexture(textureUnit);
		glBindTexture(target, texId); 
	}

	virtual ~Texture(void){}

	const std::string& getLastError() const{ return lastError; }

private:
	GLContext   context;
	GLenum      target;
	std::string fileName;

	GLuint texId;

	std::string lastError;
};

