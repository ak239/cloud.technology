#pragma once

#include "gl.h"
#include "GLContextObj.h"

template<class T>
class UniformWrapper : public GLContextObj
{
public:
	UniformWrapper() : location(-1){}
	UniformWrapper(GLuint _location) : 
		GLContextObj(), location(_location){}

	UniformWrapper(GLContext _context, GLuint _location) : 
		GLContextObj(_context), location(_location){}

	const T& getValue() const{ return val; }

	void setValue(const T& _val){
		//GLContextGetter get(getContext());
		val = _val;
		syncValue();
		//assert(glGetError() == GL_NO_ERROR);
	}

private:
	void syncValue();

	T      val;
	GLuint location;
};

template<>
inline void UniformWrapper<glm::mat4>::syncValue(){
	glUniformMatrix4fv(location, 1, GL_FALSE, &val[0][0]);
}

template<>
inline void UniformWrapper<GLuint>::syncValue(){
	glUniform1i(location, val);
}

template<>
inline void UniformWrapper<glm::vec3>::syncValue(){
	glUniform3f(location, val.x, val.y, val.z);
}

template<>
inline void UniformWrapper<glm::vec4>::syncValue(){
	glUniform4f(location, val.x, val.y, val.z, val.w);
}


template<>
inline void UniformWrapper<GLfloat>::syncValue(){
	glUniform1f(location, val);
}
